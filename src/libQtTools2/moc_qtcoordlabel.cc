/****************************************************************************
** QtCoordLabel meta object code from reading C++ file 'qtcoordlabel.hh'
**
** Created: Tue Jan 11 17:58:00 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "qtcoordlabel.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *QtCoordLabel::className() const
{
    return "QtCoordLabel";
}

QMetaObject *QtCoordLabel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QtCoordLabel( "QtCoordLabel", &QtCoordLabel::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QtCoordLabel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtCoordLabel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QtCoordLabel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtCoordLabel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QtCoordLabel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QLabel::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setCaption", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"setCoordValue", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"setCoordValue", 2, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_ptr, "QRgb", QUParameter::In }
    };
    static const QUMethod slot_3 = {"setCoordValue", 3, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_ptr, "QRgb", QUParameter::In },
	{ 0, &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_4 = {"setCoordValue", 4, param_slot_4 };
    static const QMetaData slot_tbl[] = {
	{ "setCaption(QString)", &slot_0, QMetaData::Public },
	{ "setCoordValue(int)", &slot_1, QMetaData::Public },
	{ "setCoordValue(int,int)", &slot_2, QMetaData::Public },
	{ "setCoordValue(int,int,QRgb)", &slot_3, QMetaData::Public },
	{ "setCoordValue(int,int,QRgb,float)", &slot_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"QtCoordLabel", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QtCoordLabel.setMetaObject( metaObj );
    return metaObj;
}

void* QtCoordLabel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QtCoordLabel" ) )
	return this;
    return QLabel::qt_cast( clname );
}

bool QtCoordLabel::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setCaption((QString)static_QUType_QString.get(_o+1)); break;
    case 1: setCoordValue((int)static_QUType_int.get(_o+1)); break;
    case 2: setCoordValue((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 3: setCoordValue((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(QRgb)(*((QRgb*)static_QUType_ptr.get(_o+3)))); break;
    case 4: setCoordValue((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(QRgb)(*((QRgb*)static_QUType_ptr.get(_o+3))),(float)(*((float*)static_QUType_ptr.get(_o+4)))); break;
    default:
	return QLabel::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool QtCoordLabel::qt_emit( int _id, QUObject* _o )
{
    return QLabel::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool QtCoordLabel::qt_property( int id, int f, QVariant* v)
{
    return QLabel::qt_property( id, f, v);
}

bool QtCoordLabel::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
