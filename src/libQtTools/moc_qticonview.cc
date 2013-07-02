/****************************************************************************
** QtIconView meta object code from reading C++ file 'qticonview.hh'
**
** Created: Tue Jan 11 17:58:07 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "qticonview.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *QtIconView::className() const
{
    return "QtIconView";
}

QMetaObject *QtIconView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QtIconView( "QtIconView", &QtIconView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QtIconView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtIconView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QtIconView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtIconView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QtIconView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QIconView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "img", &static_QUType_varptr, "\x0f", QUParameter::In },
	{ "sKey", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_0 = {"addImageThumbnail", 2, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "img", &static_QUType_varptr, "\x0f", QUParameter::In },
	{ "sText", &static_QUType_ptr, "string", QUParameter::In },
	{ "sKey", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_1 = {"addImageThumbnail", 3, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "img", &static_QUType_varptr, "\x06", QUParameter::In },
	{ "sText", &static_QUType_ptr, "string", QUParameter::In },
	{ "sKey", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_2 = {"addImageThumbnail", 3, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "img", &static_QUType_ptr, "OpGrayImage", QUParameter::In },
	{ "sText", &static_QUType_ptr, "string", QUParameter::In },
	{ "sKey", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_3 = {"addImageThumbnail", 3, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "button", &static_QUType_int, 0, QUParameter::In },
	{ "item", &static_QUType_ptr, "QIconViewItem", QUParameter::In },
	{ "pos", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_4 = {"mouseClickOnItem", 3, param_slot_4 };
    static const QMetaData slot_tbl[] = {
	{ "addImageThumbnail(QImage,string)", &slot_0, QMetaData::Public },
	{ "addImageThumbnail(QImage,string,string)", &slot_1, QMetaData::Public },
	{ "addImageThumbnail(QPixmap,string,string)", &slot_2, QMetaData::Public },
	{ "addImageThumbnail(OpGrayImage,string,string)", &slot_3, QMetaData::Public },
	{ "mouseClickOnItem(int,QIconViewItem*,const QPoint&)", &slot_4, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "nImgId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"leftClickOnImage", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "nImgId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"rightClickOnImage", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "nImgId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"middleClickOnImage", 1, param_signal_2 };
    static const QMetaData signal_tbl[] = {
	{ "leftClickOnImage(int)", &signal_0, QMetaData::Public },
	{ "rightClickOnImage(int)", &signal_1, QMetaData::Public },
	{ "middleClickOnImage(int)", &signal_2, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"QtIconView", parentObject,
	slot_tbl, 5,
	signal_tbl, 3,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QtIconView.setMetaObject( metaObj );
    return metaObj;
}

void* QtIconView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QtIconView" ) )
	return this;
    return QIconView::qt_cast( clname );
}

// SIGNAL leftClickOnImage
void QtIconView::leftClickOnImage( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL rightClickOnImage
void QtIconView::rightClickOnImage( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL middleClickOnImage
void QtIconView::middleClickOnImage( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

bool QtIconView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: addImageThumbnail((QImage)(*((QImage*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2)))); break;
    case 1: addImageThumbnail((QImage)(*((QImage*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2))),(string)(*((string*)static_QUType_ptr.get(_o+3)))); break;
    case 2: addImageThumbnail((QPixmap)(*((QPixmap*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2))),(string)(*((string*)static_QUType_ptr.get(_o+3)))); break;
    case 3: addImageThumbnail((OpGrayImage)(*((OpGrayImage*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2))),(string)(*((string*)static_QUType_ptr.get(_o+3)))); break;
    case 4: mouseClickOnItem((int)static_QUType_int.get(_o+1),(QIconViewItem*)static_QUType_ptr.get(_o+2),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+3))); break;
    default:
	return QIconView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool QtIconView::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: leftClickOnImage((int)static_QUType_int.get(_o+1)); break;
    case 1: rightClickOnImage((int)static_QUType_int.get(_o+1)); break;
    case 2: middleClickOnImage((int)static_QUType_int.get(_o+1)); break;
    default:
	return QIconView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool QtIconView::qt_property( int id, int f, QVariant* v)
{
    return QIconView::qt_property( id, f, v);
}

bool QtIconView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
