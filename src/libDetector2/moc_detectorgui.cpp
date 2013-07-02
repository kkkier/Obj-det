/****************************************************************************
** DetectorGUI meta object code from reading C++ file 'detectorgui.hh'
**
** Created: Tue Dec 25 06:30:05 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "detectorgui.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *DetectorGUI::className() const
{
    return "DetectorGUI";
}

QMetaObject *DetectorGUI::metaObj = 0;
static QMetaObjectCleanUp cleanUp_DetectorGUI( "DetectorGUI", &DetectorGUI::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString DetectorGUI::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DetectorGUI", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString DetectorGUI::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DetectorGUI", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* DetectorGUI::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"slotSetCategName", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"slotSetPoseName", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"slotSetAreaFactor", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"slotSetRealHeight", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"slotSetHeightVar", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"slotSetDistCenter", 1, param_slot_5 };
    static const QUMethod slot_6 = {"slotUpdateCategName", 0, 0 };
    static const QUMethod slot_7 = {"slotUpdatePoseName", 0, 0 };
    static const QUMethod slot_8 = {"slotUpdateAreaFactor", 0, 0 };
    static const QUMethod slot_9 = {"slotUpdateRealHeight", 0, 0 };
    static const QUMethod slot_10 = {"slotUpdateHeightVar", 0, 0 };
    static const QUMethod slot_11 = {"slotUpdateDistCenter", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"slotSetUseHeightVarOnOff", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"slotSetBothDirOnOff", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"slotSetApplyGPOnOff", 1, param_slot_14 };
    static const QUMethod slot_15 = {"clear", 0, 0 };
    static const QUParameter param_slot_16[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_16 = {"saveParams", 1, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_17 = {"loadParams", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"loadParams", 2, param_slot_18 };
    static const QUMethod slot_19 = {"addCue", 0, 0 };
    static const QUParameter param_slot_20[] = {
	{ "nRow", &static_QUType_int, 0, QUParameter::In },
	{ "nCol", &static_QUType_int, 0, QUParameter::In },
	{ "button", &static_QUType_int, 0, QUParameter::In },
	{ "mousePos", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_20 = {"cueTableClicked", 4, param_slot_20 };
    static const QUParameter param_slot_21[] = {
	{ "nIdx", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_21 = {"showCueWindow", 1, param_slot_21 };
    static const QUParameter param_slot_22[] = {
	{ "sDetector", &static_QUType_ptr, "string", QUParameter::In },
	{ "sFeature", &static_QUType_ptr, "string", QUParameter::In },
	{ "sNumCl", &static_QUType_ptr, "string", QUParameter::In },
	{ "sNumOcc", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_22 = {"addCueTableEntry", 4, param_slot_22 };
    static const QMetaData slot_tbl[] = {
	{ "slotSetCategName(const QString&)", &slot_0, QMetaData::Public },
	{ "slotSetPoseName(const QString&)", &slot_1, QMetaData::Public },
	{ "slotSetAreaFactor(const QString&)", &slot_2, QMetaData::Public },
	{ "slotSetRealHeight(const QString&)", &slot_3, QMetaData::Public },
	{ "slotSetHeightVar(const QString&)", &slot_4, QMetaData::Public },
	{ "slotSetDistCenter(const QString&)", &slot_5, QMetaData::Public },
	{ "slotUpdateCategName()", &slot_6, QMetaData::Public },
	{ "slotUpdatePoseName()", &slot_7, QMetaData::Public },
	{ "slotUpdateAreaFactor()", &slot_8, QMetaData::Public },
	{ "slotUpdateRealHeight()", &slot_9, QMetaData::Public },
	{ "slotUpdateHeightVar()", &slot_10, QMetaData::Public },
	{ "slotUpdateDistCenter()", &slot_11, QMetaData::Public },
	{ "slotSetUseHeightVarOnOff(int)", &slot_12, QMetaData::Public },
	{ "slotSetBothDirOnOff(int)", &slot_13, QMetaData::Public },
	{ "slotSetApplyGPOnOff(int)", &slot_14, QMetaData::Public },
	{ "clear()", &slot_15, QMetaData::Public },
	{ "saveParams(string)", &slot_16, QMetaData::Public },
	{ "loadParams(string)", &slot_17, QMetaData::Public },
	{ "loadParams(string,bool)", &slot_18, QMetaData::Public },
	{ "addCue()", &slot_19, QMetaData::Public },
	{ "cueTableClicked(int,int,int,const QPoint&)", &slot_20, QMetaData::Public },
	{ "showCueWindow(int)", &slot_21, QMetaData::Public },
	{ "addCueTableEntry(string,string,string,string)", &slot_22, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"sigCategNameChanged", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"sigPoseNameChanged", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"sigAreaFactorChanged", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"sigRealHeightChanged", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"sigHeightVarChanged", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"sigDistCenterChanged", 1, param_signal_5 };
    static const QUMethod signal_6 = {"sigAddCueClicked", 0, 0 };
    static const QUParameter param_signal_7[] = {
	{ 0, &static_QUType_ptr, "string", QUParameter::In },
	{ 0, &static_QUType_ptr, "string", QUParameter::In },
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_7 = {"sigAddCueClicked", 3, param_signal_7 };
    static const QMetaData signal_tbl[] = {
	{ "sigCategNameChanged(const QString&)", &signal_0, QMetaData::Public },
	{ "sigPoseNameChanged(const QString&)", &signal_1, QMetaData::Public },
	{ "sigAreaFactorChanged(const QString&)", &signal_2, QMetaData::Public },
	{ "sigRealHeightChanged(const QString&)", &signal_3, QMetaData::Public },
	{ "sigHeightVarChanged(const QString&)", &signal_4, QMetaData::Public },
	{ "sigDistCenterChanged(const QString&)", &signal_5, QMetaData::Public },
	{ "sigAddCueClicked()", &signal_6, QMetaData::Public },
	{ "sigAddCueClicked(string,string,bool)", &signal_7, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"DetectorGUI", parentObject,
	slot_tbl, 23,
	signal_tbl, 8,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_DetectorGUI.setMetaObject( metaObj );
    return metaObj;
}

void* DetectorGUI::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "DetectorGUI" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL sigCategNameChanged
void DetectorGUI::sigCategNameChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL sigPoseNameChanged
void DetectorGUI::sigPoseNameChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL sigAreaFactorChanged
void DetectorGUI::sigAreaFactorChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL sigRealHeightChanged
void DetectorGUI::sigRealHeightChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL sigHeightVarChanged
void DetectorGUI::sigHeightVarChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL sigDistCenterChanged
void DetectorGUI::sigDistCenterChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL sigAddCueClicked
void DetectorGUI::sigAddCueClicked()
{
    activate_signal( staticMetaObject()->signalOffset() + 6 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL sigAddCueClicked
void DetectorGUI::sigAddCueClicked( string t0, string t1, bool t2 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 7 );
    if ( !clist )
	return;
    QUObject o[4];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    static_QUType_bool.set(o+3,t2);
    activate_signal( clist, o );
}

bool DetectorGUI::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotSetCategName((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: slotSetPoseName((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: slotSetAreaFactor((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: slotSetRealHeight((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: slotSetHeightVar((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: slotSetDistCenter((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: slotUpdateCategName(); break;
    case 7: slotUpdatePoseName(); break;
    case 8: slotUpdateAreaFactor(); break;
    case 9: slotUpdateRealHeight(); break;
    case 10: slotUpdateHeightVar(); break;
    case 11: slotUpdateDistCenter(); break;
    case 12: slotSetUseHeightVarOnOff((int)static_QUType_int.get(_o+1)); break;
    case 13: slotSetBothDirOnOff((int)static_QUType_int.get(_o+1)); break;
    case 14: slotSetApplyGPOnOff((int)static_QUType_int.get(_o+1)); break;
    case 15: clear(); break;
    case 16: saveParams((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 17: loadParams((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 18: loadParams((string)(*((string*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 19: addCue(); break;
    case 20: cueTableClicked((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 21: showCueWindow((int)static_QUType_int.get(_o+1)); break;
    case 22: addCueTableEntry((string)(*((string*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2))),(string)(*((string*)static_QUType_ptr.get(_o+3))),(string)(*((string*)static_QUType_ptr.get(_o+4)))); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool DetectorGUI::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigCategNameChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: sigPoseNameChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: sigAreaFactorChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: sigRealHeightChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: sigHeightVarChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: sigDistCenterChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: sigAddCueClicked(); break;
    case 7: sigAddCueClicked((string)(*((string*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2))),(bool)static_QUType_bool.get(_o+3)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool DetectorGUI::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool DetectorGUI::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
