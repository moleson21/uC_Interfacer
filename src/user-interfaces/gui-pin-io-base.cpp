/*
 * uC Interface - A GUI for Programming & Interfacing with Microcontrollers
 * Copyright (C) 2018  Mitchell Oleson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "gui-pin-io-base.h"

GUI_PIN_BASE::GUI_PIN_BASE(QWidget *parent) :
    GUI_BASE(parent)
{
}

GUI_PIN_BASE::~GUI_PIN_BASE()
{
}

void GUI_PIN_BASE::addNewPinSettings(uint8_t pinType, QList<QString> newSettings)
{
//    qDebug() << newSettings;
    QList<QString> settingValues;
    QList<QString> pinCombos = {}, pinSetDisabled = {};
    QList<RangeList> pinRanges = {};
    foreach (QString i, newSettings)
    {
        // Split settings string into values [name,setEnabled,rangeList]
        settingValues = (i).split(',');

        pinCombos.append(settingValues[0]);

        if (settingValues[1].toLower() == "true")
            pinSetDisabled.append(settingValues[0]);

        pinRanges.append(*makeRangeList(settingValues[2]));
    }

    // Add new pinTypes (if relevant)
    if (!controlMap.contains(pinType)) controlMap.insert(pinType, new QMap<QString, uint8_t>());
    if (!disabledValueSet.contains(pinType)) disabledValueSet.insert(pinType, new QList<uint8_t>());
    if (!rangeMap.contains(pinType)) rangeMap.insert(pinType, new QMap<uint8_t, RangeList>());

//    qDebug() << pinCombos;
//    qDebug() << pinSetDisabled;

    addPinControls(pinType, pinCombos);
    addPinRangeMap(pinType, pinCombos, pinRanges);

    QMap<QString, uint8_t>* pinMap = controlMap.value(pinType);
    QList<uint8_t>* pinDisabledSet = disabledValueSet.value(pinType);
    foreach (QString i, pinSetDisabled)
    {
        pinDisabledSet->append(pinMap->value(i));
    }
}

void GUI_PIN_BASE::inputsChanged(PinTypeInfo *pInfo, int colOffset)
{
    // Get info of set button clicked
    QObject *caller = sender();
    int in, col, row, rowSp, colSp;
    in = pInfo->grid->indexOf((QWidget*) caller);
    pInfo->grid->getItemPosition(in, &row, &col, &rowSp, &colSp);
    col = col - colOffset;

    // Get all pin info
    QLayoutItem *itemLabel = pInfo->grid->itemAtPosition(row, col+labelPos);
    QLayoutItem *itemCombo = pInfo->grid->itemAtPosition(row, col+comboPos);
    QLayoutItem *itemSliderValue = pInfo->grid->itemAtPosition(row, col+slideValuePos);
    QLayoutItem *itemTextValue = pInfo->grid->itemAtPosition(row, col+textValuePos);
    if (!(itemLabel && itemCombo && itemSliderValue && itemTextValue)) return;

    // Get mapping info
    QMap<QString, uint8_t>* pinMap = controlMap.value(pInfo->pinType);
    QMap<uint8_t, RangeList>* pinRangeMap = rangeMap.value(pInfo->pinType);
    QList<uint8_t>* pinDisabledSet = disabledValueSet.value(pInfo->pinType);
    if (!pinMap || !pinRangeMap || !pinDisabledSet) return;

    // Find widgets of interest
    QLabel *label = (QLabel*) itemLabel->widget();
    QComboBox *combo = (QComboBox*) itemCombo->widget();
    QSlider *sliderValue = (QSlider*) itemSliderValue->widget();
    QLineEdit *textValue = (QLineEdit*) itemTextValue->widget();
    if (!(label && combo && sliderValue && textValue)) return;

    // Set Pin Num
    QString pinNum = QString::number(label->text().toInt());

    // Clarify IO selection
    uint8_t IO = pinMap->value(combo->currentText());

    // Get range list for use in next sections
    RangeList rList = pinRangeMap->value(IO);

    // Set IO if combo changed
    if (caller == combo)
    {
        // Enable/Disable pins if selection changed
        if (pinDisabledSet->contains(IO))
        {
            sliderValue->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            textValue->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        } else
        {
            sliderValue->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            textValue->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        }

        // Reset slider range if selection changed
        int prevPos = sliderValue->sliderPosition();
        updateSliderRange(sliderValue, &rList);

        int curPos = sliderValue->sliderPosition();
        if (prevPos == curPos) emit sliderValue->valueChanged(curPos);

        return;
    }

    QString VAL;
    if (caller == sliderValue)
    {
        // Update values if slider moved
        int newVal = sliderValue->value();
        float tempVAL = (float) newVal / rList.div;

        if (pInfo->pinType == JSON_AIO) tempVAL = qRound(tempVAL);

        VAL = QString::number(tempVAL);
        textValue->setText(VAL);
    } else if (caller == textValue)
    {
        // Update values if text box changed
        VAL = textValue->text();
        float tempVAL = rList.div * VAL.toFloat();

        if (pInfo->pinType != JSON_AIO) tempVAL = qRound(tempVAL);

        sliderValue->setSliderPosition(tempVAL);
        VAL = QString::number(tempVAL);
        return;
    } else
    {
        // No change or error
        return;
    }

    // Send request to uC
    uint16_t v = (uint16_t) VAL.toInt();
    send({
             pInfo->pinType,
             (uint8_t) pinNum.toInt(),
             IO,
             (uint8_t) ((v >> 8) & 0xFF),
             (uint8_t) (v & 0xFF)
         });
}

void GUI_PIN_BASE::updateSliderRange(QSlider *slider, RangeList *rList)
{
    slider->setMinimum(rList->min);
    slider->setMaximum(rList->max);
    slider->setTickInterval(rList->step);
    slider->setSingleStep(rList->step);
    slider->setPageStep(rList->step);

    if (0 < rList->min) slider->setSliderPosition(rList->min);
    else if (rList->max < 0) slider->setSliderPosition(rList->max);
    else slider->setSliderPosition(0);
}

void GUI_PIN_BASE::setPinAttribute(PinTypeInfo *pInfo, int pinNum, Qt::WidgetAttribute attribute, bool on)
{
    // Find row & column of desired buttons
    int rowNum, colNum;
    getPinLocation(&rowNum, &colNum, pInfo, pinNum);

    // Go through each element and set attributes
    QWidget *itemWidget;
    for (int i = 0; i < pInfo->numButtons; i++)
    {
        if (getItemWidget(&itemWidget, pInfo->grid, rowNum, colNum+i))
        {
            itemWidget->setAttribute(attribute, on);
        }
    }
}

bool GUI_PIN_BASE::getItemWidget(QWidget** itemWidget, QGridLayout *grid, int row, int col)
{
    *itemWidget = 0;
    QLayoutItem *item = grid->itemAtPosition(row, col);
    if (item) *itemWidget = item->widget();
    return (*itemWidget != 0);
}

void GUI_PIN_BASE::getPinLocation(int *row, int *col, PinTypeInfo *pInfo, int pin)
{
    *row = pin / pInfo->cols;
    *col = pInfo->numButtons * (pin % pInfo->cols);
}

bool GUI_PIN_BASE::getPinTypeInfo(uint8_t pinType, PinTypeInfo *infoPtr)
{
    infoPtr->pinType = pinType;

    // Set pin type variables
    switch (pinType)
    {
        case JSON_AIO:
            infoPtr->numButtons = num_AIObuttons;
            infoPtr->numPins = num_AIOpins_GUI;
            infoPtr->cols = num_AIOcols;
            infoPtr->rows = num_AIOrows;
            return true;
        case JSON_DIO:
            infoPtr->numButtons = num_DIObuttons;
            infoPtr->numPins = num_DIOpins_GUI;
            infoPtr->cols = num_DIOcols;
            infoPtr->rows = num_DIOrows;
            return true;
        default:
            delete infoPtr;
            *infoPtr = EMPTY_PIN_TYPE_INFO;
            return false;
    }
}

RangeList* GUI_PIN_BASE::makeRangeList(QString rangeInfo)
{
    // Split range info string into values
    QStringList rangeInfoList = rangeInfo.split('-');
    if (rangeInfoList.length() != 4) return new EMPTY_RANGE;

    RangeList* rList = new RangeList();

    // Set min
    if (rangeInfoList[0].isEmpty()) rList->min = 0;
    else rList->min = rangeInfoList[0].toInt();

    // Set max
    if (rangeInfoList[1].isEmpty()) rList->max = 0;
    else rList->max = rangeInfoList[1].toInt();

    // Set step
    if (rangeInfoList[2].isEmpty()) rList->step = 0;
    else rList->step = rangeInfoList[2].toInt();

    // Set div
    if (rangeInfoList[3].isEmpty()) rList->div = 0;
    else rList->div = rangeInfoList[3].toFloat();

    return rList;
}

void GUI_PIN_BASE::addPinControls(uint8_t pinType, QList<QString> keys)
{
    QMap<QString, uint8_t>* pinMap = controlMap.value(pinType);
    uint8_t key_num = pinMap->keys().length();
    foreach (QString i, keys)
    {
        pinMap->insert(i, key_num);
        key_num += 1;
    }
}

void GUI_PIN_BASE::addPinRangeMap(uint8_t pinType, QList<QString> keys, QList<RangeList> values)
{
    if (keys.length() != values.length()) return;

    QMap<QString, uint8_t>* pinMap = controlMap.value(pinType);
    QMap<uint8_t, RangeList>* pinRangeMap = rangeMap.value(pinType);
    for (int i = 0; i < keys.length(); i++)
    {
        pinRangeMap->insert(pinMap->value(keys[i]), values[i]);
    }
}