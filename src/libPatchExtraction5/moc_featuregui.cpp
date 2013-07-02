/****************************************************************************
** FeatureGUI meta object code from reading C++ file 'featuregui.hh'
**
** Created: Fri Jan 25 17:17:47 2013
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "featuregui.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *FeatureGUI::className() const
{
    return "FeatureGUI";
}

QMetaObject *FeatureGUI::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FeatureGUI( "FeatureGUI", &FeatureGUI::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FeatureGUI::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FeatureGUI", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FeatureGUI::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FeatureGUI", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FeatureGUI::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"slotSetSigma1", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"slotSetSigma2", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"slotSetAlpha", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"slotSetThreshHar", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"slotSetScaleOctaves", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"slotSetLevsPerOctave", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"slotSetScaleSigma0", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"slotSetThreshEdog", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"slotSetThreshSURF", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"slotSetPatchSize", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"slotSetMinScale", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"slotSetMaxScale", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"slotSetStepSize", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"slotSetMinFigurePixels", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"slotSetFigureThresh", 1, param_slot_14 };
    static const QUMethod slot_15 = {"slotUpdateSigma1", 0, 0 };
    static const QUMethod slot_16 = {"slotUpdateSigma2", 0, 0 };
    static const QUMethod slot_17 = {"slotUpdateAlpha", 0, 0 };
    static const QUMethod slot_18 = {"slotUpdateThreshHar", 0, 0 };
    static const QUMethod slot_19 = {"slotUpdateScaleOctaves", 0, 0 };
    static const QUMethod slot_20 = {"slotUpdateLevsPerOctave", 0, 0 };
    static const QUMethod slot_21 = {"slotUpdateScaleSigma0", 0, 0 };
    static const QUMethod slot_22 = {"slotUpdateThreshEdog", 0, 0 };
    static const QUMethod slot_23 = {"slotUpdateThreshSURF", 0, 0 };
    static const QUMethod slot_24 = {"slotUpdatePatchSize", 0, 0 };
    static const QUMethod slot_25 = {"slotUpdateMinScale", 0, 0 };
    static const QUMethod slot_26 = {"slotUpdateMaxScale", 0, 0 };
    static const QUMethod slot_27 = {"slotUpdateStepSize", 0, 0 };
    static const QUMethod slot_28 = {"slotUpdateMinFigurePixels", 0, 0 };
    static const QUMethod slot_29 = {"slotUpdateFigureThresh", 0, 0 };
    static const QUParameter param_slot_30[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_30 = {"slotSelectPatchExtMethod", 1, param_slot_30 };
    static const QUParameter param_slot_31[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_31 = {"slotSelectFeatureType", 1, param_slot_31 };
    static const QUParameter param_slot_32[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_32 = {"slotSelectNormalizeMethod", 1, param_slot_32 };
    static const QUParameter param_slot_33[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_33 = {"slotSetUseFigureOnlyOnOff", 1, param_slot_33 };
    static const QUParameter param_slot_34[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_34 = {"slotSetFilterPatchesOnOff", 1, param_slot_34 };
    static const QUParameter param_slot_35[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_35 = {"slotSetMakeRotInvOnOff", 1, param_slot_35 };
    static const QUParameter param_slot_36[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_36 = {"saveParams", 1, param_slot_36 };
    static const QUParameter param_slot_37[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_37 = {"saveParams", 2, param_slot_37 };
    static const QUParameter param_slot_38[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_38 = {"loadParams", 1, param_slot_38 };
    static const QUParameter param_slot_39[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_39 = {"loadParams", 2, param_slot_39 };
    static const QMetaData slot_tbl[] = {
	{ "slotSetSigma1(const QString&)", &slot_0, QMetaData::Public },
	{ "slotSetSigma2(const QString&)", &slot_1, QMetaData::Public },
	{ "slotSetAlpha(const QString&)", &slot_2, QMetaData::Public },
	{ "slotSetThreshHar(const QString&)", &slot_3, QMetaData::Public },
	{ "slotSetScaleOctaves(const QString&)", &slot_4, QMetaData::Public },
	{ "slotSetLevsPerOctave(const QString&)", &slot_5, QMetaData::Public },
	{ "slotSetScaleSigma0(const QString&)", &slot_6, QMetaData::Public },
	{ "slotSetThreshEdog(const QString&)", &slot_7, QMetaData::Public },
	{ "slotSetThreshSURF(const QString&)", &slot_8, QMetaData::Public },
	{ "slotSetPatchSize(const QString&)", &slot_9, QMetaData::Public },
	{ "slotSetMinScale(const QString&)", &slot_10, QMetaData::Public },
	{ "slotSetMaxScale(const QString&)", &slot_11, QMetaData::Public },
	{ "slotSetStepSize(const QString&)", &slot_12, QMetaData::Public },
	{ "slotSetMinFigurePixels(const QString&)", &slot_13, QMetaData::Public },
	{ "slotSetFigureThresh(const QString&)", &slot_14, QMetaData::Public },
	{ "slotUpdateSigma1()", &slot_15, QMetaData::Public },
	{ "slotUpdateSigma2()", &slot_16, QMetaData::Public },
	{ "slotUpdateAlpha()", &slot_17, QMetaData::Public },
	{ "slotUpdateThreshHar()", &slot_18, QMetaData::Public },
	{ "slotUpdateScaleOctaves()", &slot_19, QMetaData::Public },
	{ "slotUpdateLevsPerOctave()", &slot_20, QMetaData::Public },
	{ "slotUpdateScaleSigma0()", &slot_21, QMetaData::Public },
	{ "slotUpdateThreshEdog()", &slot_22, QMetaData::Public },
	{ "slotUpdateThreshSURF()", &slot_23, QMetaData::Public },
	{ "slotUpdatePatchSize()", &slot_24, QMetaData::Public },
	{ "slotUpdateMinScale()", &slot_25, QMetaData::Public },
	{ "slotUpdateMaxScale()", &slot_26, QMetaData::Public },
	{ "slotUpdateStepSize()", &slot_27, QMetaData::Public },
	{ "slotUpdateMinFigurePixels()", &slot_28, QMetaData::Public },
	{ "slotUpdateFigureThresh()", &slot_29, QMetaData::Public },
	{ "slotSelectPatchExtMethod(int)", &slot_30, QMetaData::Public },
	{ "slotSelectFeatureType(int)", &slot_31, QMetaData::Public },
	{ "slotSelectNormalizeMethod(int)", &slot_32, QMetaData::Public },
	{ "slotSetUseFigureOnlyOnOff(int)", &slot_33, QMetaData::Public },
	{ "slotSetFilterPatchesOnOff(int)", &slot_34, QMetaData::Public },
	{ "slotSetMakeRotInvOnOff(int)", &slot_35, QMetaData::Public },
	{ "saveParams(string)", &slot_36, QMetaData::Public },
	{ "saveParams(string,bool)", &slot_37, QMetaData::Public },
	{ "loadParams(string)", &slot_38, QMetaData::Public },
	{ "loadParams(string,bool)", &slot_39, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"sigSigma1Changed", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"sigSigma2Changed", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"sigAlphaChanged", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"sigThreshHarChanged", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"sigScaleOctavesChanged", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"sigLevsPerOctaveChanged", 1, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_6 = {"sigScaleSigma0Changed", 1, param_signal_6 };
    static const QUParameter param_signal_7[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_7 = {"sigThreshEdogChanged", 1, param_signal_7 };
    static const QUParameter param_signal_8[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_8 = {"sigThreshSURFChanged", 1, param_signal_8 };
    static const QUParameter param_signal_9[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_9 = {"sigPatchSizeChanged", 1, param_signal_9 };
    static const QUParameter param_signal_10[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_10 = {"sigMinScaleChanged", 1, param_signal_10 };
    static const QUParameter param_signal_11[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_11 = {"sigMaxScaleChanged", 1, param_signal_11 };
    static const QUParameter param_signal_12[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_12 = {"sigStepSizeChanged", 1, param_signal_12 };
    static const QUParameter param_signal_13[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_13 = {"sigMinFigurePixelsChanged", 1, param_signal_13 };
    static const QUParameter param_signal_14[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_14 = {"sigFigureThreshChanged", 1, param_signal_14 };
    static const QMetaData signal_tbl[] = {
	{ "sigSigma1Changed(const QString&)", &signal_0, QMetaData::Public },
	{ "sigSigma2Changed(const QString&)", &signal_1, QMetaData::Public },
	{ "sigAlphaChanged(const QString&)", &signal_2, QMetaData::Public },
	{ "sigThreshHarChanged(const QString&)", &signal_3, QMetaData::Public },
	{ "sigScaleOctavesChanged(const QString&)", &signal_4, QMetaData::Public },
	{ "sigLevsPerOctaveChanged(const QString&)", &signal_5, QMetaData::Public },
	{ "sigScaleSigma0Changed(const QString&)", &signal_6, QMetaData::Public },
	{ "sigThreshEdogChanged(const QString&)", &signal_7, QMetaData::Public },
	{ "sigThreshSURFChanged(const QString&)", &signal_8, QMetaData::Public },
	{ "sigPatchSizeChanged(const QString&)", &signal_9, QMetaData::Public },
	{ "sigMinScaleChanged(const QString&)", &signal_10, QMetaData::Public },
	{ "sigMaxScaleChanged(const QString&)", &signal_11, QMetaData::Public },
	{ "sigStepSizeChanged(const QString&)", &signal_12, QMetaData::Public },
	{ "sigMinFigurePixelsChanged(const QString&)", &signal_13, QMetaData::Public },
	{ "sigFigureThreshChanged(const QString&)", &signal_14, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"FeatureGUI", parentObject,
	slot_tbl, 40,
	signal_tbl, 15,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FeatureGUI.setMetaObject( metaObj );
    return metaObj;
}

void* FeatureGUI::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FeatureGUI" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL sigSigma1Changed
void FeatureGUI::sigSigma1Changed( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL sigSigma2Changed
void FeatureGUI::sigSigma2Changed( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL sigAlphaChanged
void FeatureGUI::sigAlphaChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL sigThreshHarChanged
void FeatureGUI::sigThreshHarChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL sigScaleOctavesChanged
void FeatureGUI::sigScaleOctavesChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL sigLevsPerOctaveChanged
void FeatureGUI::sigLevsPerOctaveChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL sigScaleSigma0Changed
void FeatureGUI::sigScaleSigma0Changed( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 6, t0 );
}

// SIGNAL sigThreshEdogChanged
void FeatureGUI::sigThreshEdogChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 7, t0 );
}

// SIGNAL sigThreshSURFChanged
void FeatureGUI::sigThreshSURFChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 8, t0 );
}

// SIGNAL sigPatchSizeChanged
void FeatureGUI::sigPatchSizeChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 9, t0 );
}

// SIGNAL sigMinScaleChanged
void FeatureGUI::sigMinScaleChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 10, t0 );
}

// SIGNAL sigMaxScaleChanged
void FeatureGUI::sigMaxScaleChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 11, t0 );
}

// SIGNAL sigStepSizeChanged
void FeatureGUI::sigStepSizeChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 12, t0 );
}

// SIGNAL sigMinFigurePixelsChanged
void FeatureGUI::sigMinFigurePixelsChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 13, t0 );
}

// SIGNAL sigFigureThreshChanged
void FeatureGUI::sigFigureThreshChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 14, t0 );
}

bool FeatureGUI::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotSetSigma1((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: slotSetSigma2((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: slotSetAlpha((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: slotSetThreshHar((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: slotSetScaleOctaves((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: slotSetLevsPerOctave((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: slotSetScaleSigma0((const QString&)static_QUType_QString.get(_o+1)); break;
    case 7: slotSetThreshEdog((const QString&)static_QUType_QString.get(_o+1)); break;
    case 8: slotSetThreshSURF((const QString&)static_QUType_QString.get(_o+1)); break;
    case 9: slotSetPatchSize((const QString&)static_QUType_QString.get(_o+1)); break;
    case 10: slotSetMinScale((const QString&)static_QUType_QString.get(_o+1)); break;
    case 11: slotSetMaxScale((const QString&)static_QUType_QString.get(_o+1)); break;
    case 12: slotSetStepSize((const QString&)static_QUType_QString.get(_o+1)); break;
    case 13: slotSetMinFigurePixels((const QString&)static_QUType_QString.get(_o+1)); break;
    case 14: slotSetFigureThresh((const QString&)static_QUType_QString.get(_o+1)); break;
    case 15: slotUpdateSigma1(); break;
    case 16: slotUpdateSigma2(); break;
    case 17: slotUpdateAlpha(); break;
    case 18: slotUpdateThreshHar(); break;
    case 19: slotUpdateScaleOctaves(); break;
    case 20: slotUpdateLevsPerOctave(); break;
    case 21: slotUpdateScaleSigma0(); break;
    case 22: slotUpdateThreshEdog(); break;
    case 23: slotUpdateThreshSURF(); break;
    case 24: slotUpdatePatchSize(); break;
    case 25: slotUpdateMinScale(); break;
    case 26: slotUpdateMaxScale(); break;
    case 27: slotUpdateStepSize(); break;
    case 28: slotUpdateMinFigurePixels(); break;
    case 29: slotUpdateFigureThresh(); break;
    case 30: slotSelectPatchExtMethod((int)static_QUType_int.get(_o+1)); break;
    case 31: slotSelectFeatureType((int)static_QUType_int.get(_o+1)); break;
    case 32: slotSelectNormalizeMethod((int)static_QUType_int.get(_o+1)); break;
    case 33: slotSetUseFigureOnlyOnOff((int)static_QUType_int.get(_o+1)); break;
    case 34: slotSetFilterPatchesOnOff((int)static_QUType_int.get(_o+1)); break;
    case 35: slotSetMakeRotInvOnOff((int)static_QUType_int.get(_o+1)); break;
    case 36: saveParams((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 37: saveParams((string)(*((string*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 38: loadParams((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 39: loadParams((string)(*((string*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool FeatureGUI::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigSigma1Changed((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: sigSigma2Changed((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: sigAlphaChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: sigThreshHarChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: sigScaleOctavesChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: sigLevsPerOctaveChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: sigScaleSigma0Changed((const QString&)static_QUType_QString.get(_o+1)); break;
    case 7: sigThreshEdogChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 8: sigThreshSURFChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 9: sigPatchSizeChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 10: sigMinScaleChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 11: sigMaxScaleChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 12: sigStepSizeChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 13: sigMinFigurePixelsChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 14: sigFigureThreshChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool FeatureGUI::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool FeatureGUI::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
