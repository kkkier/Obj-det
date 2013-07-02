/****************************************************************************
** VeriGUI meta object code from reading C++ file 'verigui.hh'
**
** Created: Tue Dec 25 06:30:21 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "verigui.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *VeriGUI::className() const
{
    return "VeriGUI";
}

QMetaObject *VeriGUI::metaObj = 0;
static QMetaObjectCleanUp cleanUp_VeriGUI( "VeriGUI", &VeriGUI::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString VeriGUI::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "VeriGUI", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString VeriGUI::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "VeriGUI", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* VeriGUI::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"slotSetVerThreshSingle", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"slotSetVerMinPFig", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"slotSetVerWeightPFig", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"slotSetVerChAlpha", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"slotSetVerChThresh", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"slotSetTemplateScale", 1, param_slot_5 };
    static const QUMethod slot_6 = {"slotUpdateVerThreshSingle", 0, 0 };
    static const QUMethod slot_7 = {"slotUpdateVerMinPFig", 0, 0 };
    static const QUMethod slot_8 = {"slotUpdateVerWeightPFig", 0, 0 };
    static const QUMethod slot_9 = {"slotUpdateVerChAlpha", 0, 0 };
    static const QUMethod slot_10 = {"slotUpdateVerChThresh", 0, 0 };
    static const QUMethod slot_11 = {"slotUpdateTemplateScale", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"slotSelectVerifMethod", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"slotSetDoVerifOnOff", 1, param_slot_13 };
    static const QMetaData slot_tbl[] = {
	{ "slotSetVerThreshSingle(const QString&)", &slot_0, QMetaData::Public },
	{ "slotSetVerMinPFig(const QString&)", &slot_1, QMetaData::Public },
	{ "slotSetVerWeightPFig(const QString&)", &slot_2, QMetaData::Public },
	{ "slotSetVerChAlpha(const QString&)", &slot_3, QMetaData::Public },
	{ "slotSetVerChThresh(const QString&)", &slot_4, QMetaData::Public },
	{ "slotSetTemplateScale(const QString&)", &slot_5, QMetaData::Public },
	{ "slotUpdateVerThreshSingle()", &slot_6, QMetaData::Public },
	{ "slotUpdateVerMinPFig()", &slot_7, QMetaData::Public },
	{ "slotUpdateVerWeightPFig()", &slot_8, QMetaData::Public },
	{ "slotUpdateVerChAlpha()", &slot_9, QMetaData::Public },
	{ "slotUpdateVerChThresh()", &slot_10, QMetaData::Public },
	{ "slotUpdateTemplateScale()", &slot_11, QMetaData::Public },
	{ "slotSelectVerifMethod(int)", &slot_12, QMetaData::Public },
	{ "slotSetDoVerifOnOff(int)", &slot_13, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"sigVerThreshSingleChanged", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"sigVerMinPFigChanged", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"sigVerWeightPFigChanged", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"sigVerChAlphaChanged", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"sigVerChThreshChanged", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"sigTemplateScaleChanged", 1, param_signal_5 };
    static const QMetaData signal_tbl[] = {
	{ "sigVerThreshSingleChanged(const QString&)", &signal_0, QMetaData::Public },
	{ "sigVerMinPFigChanged(const QString&)", &signal_1, QMetaData::Public },
	{ "sigVerWeightPFigChanged(const QString&)", &signal_2, QMetaData::Public },
	{ "sigVerChAlphaChanged(const QString&)", &signal_3, QMetaData::Public },
	{ "sigVerChThreshChanged(const QString&)", &signal_4, QMetaData::Public },
	{ "sigTemplateScaleChanged(const QString&)", &signal_5, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"VeriGUI", parentObject,
	slot_tbl, 14,
	signal_tbl, 6,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_VeriGUI.setMetaObject( metaObj );
    return metaObj;
}

void* VeriGUI::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "VeriGUI" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL sigVerThreshSingleChanged
void VeriGUI::sigVerThreshSingleChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL sigVerMinPFigChanged
void VeriGUI::sigVerMinPFigChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL sigVerWeightPFigChanged
void VeriGUI::sigVerWeightPFigChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL sigVerChAlphaChanged
void VeriGUI::sigVerChAlphaChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL sigVerChThreshChanged
void VeriGUI::sigVerChThreshChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL sigTemplateScaleChanged
void VeriGUI::sigTemplateScaleChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

bool VeriGUI::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotSetVerThreshSingle((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: slotSetVerMinPFig((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: slotSetVerWeightPFig((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: slotSetVerChAlpha((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: slotSetVerChThresh((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: slotSetTemplateScale((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: slotUpdateVerThreshSingle(); break;
    case 7: slotUpdateVerMinPFig(); break;
    case 8: slotUpdateVerWeightPFig(); break;
    case 9: slotUpdateVerChAlpha(); break;
    case 10: slotUpdateVerChThresh(); break;
    case 11: slotUpdateTemplateScale(); break;
    case 12: slotSelectVerifMethod((int)static_QUType_int.get(_o+1)); break;
    case 13: slotSetDoVerifOnOff((int)static_QUType_int.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool VeriGUI::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigVerThreshSingleChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: sigVerMinPFigChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: sigVerWeightPFigChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: sigVerChAlphaChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: sigVerChThreshChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: sigTemplateScaleChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool VeriGUI::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool VeriGUI::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
