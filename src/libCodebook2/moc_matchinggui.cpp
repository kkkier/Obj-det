/****************************************************************************
** MatchingGUI meta object code from reading C++ file 'matchinggui.hh'
**
** Created: Wed Jan 30 12:05:32 2013
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "matchinggui.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MatchingGUI::className() const
{
    return "MatchingGUI";
}

QMetaObject *MatchingGUI::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MatchingGUI( "MatchingGUI", &MatchingGUI::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MatchingGUI::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MatchingGUI", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MatchingGUI::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MatchingGUI", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MatchingGUI::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"slotSetRejectionThreshold", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"slotSetFeatureSimFact", 1, param_slot_1 };
    static const QUMethod slot_2 = {"slotUpdateRejectionThreshold", 0, 0 };
    static const QUMethod slot_3 = {"slotUpdateFeatureSimFact", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"slotSelectCompMethod", 1, param_slot_4 };
    static const QMetaData slot_tbl[] = {
	{ "slotSetRejectionThreshold(const QString&)", &slot_0, QMetaData::Public },
	{ "slotSetFeatureSimFact(const QString&)", &slot_1, QMetaData::Public },
	{ "slotUpdateRejectionThreshold()", &slot_2, QMetaData::Public },
	{ "slotUpdateFeatureSimFact()", &slot_3, QMetaData::Public },
	{ "slotSelectCompMethod(int)", &slot_4, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"sigRejectionThresholdChanged", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"sigFeatureSimFactChanged", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "sigRejectionThresholdChanged(const QString&)", &signal_0, QMetaData::Public },
	{ "sigFeatureSimFactChanged(const QString&)", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MatchingGUI", parentObject,
	slot_tbl, 5,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MatchingGUI.setMetaObject( metaObj );
    return metaObj;
}

void* MatchingGUI::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MatchingGUI" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL sigRejectionThresholdChanged
void MatchingGUI::sigRejectionThresholdChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL sigFeatureSimFactChanged
void MatchingGUI::sigFeatureSimFactChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

bool MatchingGUI::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotSetRejectionThreshold((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: slotSetFeatureSimFact((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: slotUpdateRejectionThreshold(); break;
    case 3: slotUpdateFeatureSimFact(); break;
    case 4: slotSelectCompMethod((int)static_QUType_int.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MatchingGUI::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigRejectionThresholdChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: sigFeatureSimFactChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MatchingGUI::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MatchingGUI::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
