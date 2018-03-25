#ifndef GUI_8AIO_16DIO_COMM_H
#define GUI_8AIO_16DIO_COMM_H

#include "GUI_PIN_BASE.h"

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QSlider>
#include <QTextStream>

namespace Ui {
class GUI_8AIO_16DIO_COMM;
}

class GUI_8AIO_16DIO_COMM : public GUI_PIN_BASE
{
    Q_OBJECT

public:
    explicit GUI_8AIO_16DIO_COMM(QStringList params, QWidget *parent = 0);
    ~GUI_8AIO_16DIO_COMM();

private slots:
    void on_RESEST_BUTTON_clicked();

    void DIO_ComboChanged();
    void DIO_SliderValueChanged();
    void DIO_TextValueChanged();

    void AIO_ComboChanged();
    void AIO_SliderValueChanged();
    void AIO_TextValueChanged();

    void updateValues();
    void receive(QByteArray recvData);
    void recordLogData();

    void on_updateStarter_clicked();
    void on_updateStopper_clicked();

    void on_selectSaveLocation_clicked();
    void on_startLog_clicked();
    void on_stopLog_clicked();

    void on_ConnectButton_clicked();
    void on_SendButton_clicked();
    void on_ClearRecvButton_clicked();

    void on_ConnTypeCombo_currentIndexChanged(int);

protected:
    Ui::GUI_8AIO_16DIO_COMM *ui;

    void addNewPinSettings(QList<uint8_t> pinTypes, QList<QString> pinCombos, QList<uint8_t> pinValues, QList<bool> pinSetDisabled, QList<RangeList> pinRanges = {EMPTY_RANGE});

    void disablePins(uint8_t pinType, QList<int> pinNums);
    void setCombos(uint8_t pinTypes, QList<QString> values, QList<int> pinNums = {});

    void setConTypes(QStringList connTypes, QList<char> mapValues);

private:
    void initialize();
    void setupUpdaters();

    void connectUniversalSlots();

    void setValues(uint8_t pinType, QByteArray values);

    void recordPinValues(PinTypeInfo *pInfo);

    void addPinControls(QList<QString> keys, QList<uint8_t> values);
    void addPinRangeMap(QList<uint8_t> keys, QList<RangeList> values = {EMPTY_RANGE});

    bool getPinTypeInfo(uint8_t pinType, PinTypeInfo *infoPtr);
};

#endif // GUI_8AIO_16DIO_COMM_H
