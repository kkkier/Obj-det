/****************************************************************************
** CueWidget meta object code from reading C++ file 'cuewidget.hh'
**
** Created: Tue Dec 25 06:30:05 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "cuewidget.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CueWidget::className() const
{
    return "CueWidget";
}

QMetaObject *CueWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CueWidget( "CueWidget", &CueWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CueWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CueWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CueWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CueWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CueWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"propagateCueUpdate", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "propagateCueUpdate(const QString&)", &slot_0, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"sigCuesChanged", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "sigCuesChanged()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CueWidget", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CueWidget.setMetaObject( metaObj );
    return metaObj;
}

void* CueWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CueWidget" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL sigCuesChanged
void CueWidget::sigCuesChanged()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool CueWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: propagateCueUpdate((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CueWidget::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigCuesChanged(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CueWidget::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool CueWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
