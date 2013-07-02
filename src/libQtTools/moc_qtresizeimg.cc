/****************************************************************************
** QtResizeImg meta object code from reading C++ file 'qtresizeimg.hh'
**
** Created: Tue Jan 11 17:57:57 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "qtresizeimg.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *QtResizeImg::className() const
{
    return "QtResizeImg";
}

QMetaObject *QtResizeImg::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QtResizeImg( "QtResizeImg", &QtResizeImg::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QtResizeImg::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtResizeImg", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QtResizeImg::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtResizeImg", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QtResizeImg::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "w", &static_QUType_int, 0, QUParameter::In },
	{ "h", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setSize", 2, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "size", &static_QUType_varptr, "\x09", QUParameter::In }
    };
    static const QUMethod slot_1 = {"setSize", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "sx", &static_QUType_ptr, "float", QUParameter::In },
	{ "sy", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_2 = {"setScale", 2, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "img", &static_QUType_varptr, "\x0f", QUParameter::In }
    };
    static const QUMethod slot_3 = {"loadImage", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "pixmap", &static_QUType_varptr, "\x06", QUParameter::In }
    };
    static const QUMethod slot_4 = {"loadImage", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "img", &static_QUType_varptr, "\x0f", QUParameter::In },
	{ "grayImg", &static_QUType_ptr, "GrayImage", QUParameter::InOut }
    };
    static const QUMethod slot_5 = {"loadImage", 2, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "pixmap", &static_QUType_varptr, "\x06", QUParameter::In },
	{ "grayImg", &static_QUType_ptr, "GrayImage", QUParameter::InOut }
    };
    static const QUMethod slot_6 = {"loadImage", 2, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "filename", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"loadImage", 1, param_slot_7 };
    static const QUMethod slot_8 = {"loadTestImage", 0, 0 };
    static const QUMethod slot_9 = {"display", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ "r", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"setSelectRadius", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "x", &static_QUType_int, 0, QUParameter::In },
	{ "y", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"setSelectRadius", 2, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "color", &static_QUType_varptr, "\x0a", QUParameter::InOut }
    };
    static const QUMethod slot_12 = {"setSelectColor", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "x", &static_QUType_int, 0, QUParameter::In },
	{ "y", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"selectImgRegion", 2, param_slot_13 };
    static const QMetaData slot_tbl[] = {
	{ "setSize(int,int)", &slot_0, QMetaData::Public },
	{ "setSize(QSize)", &slot_1, QMetaData::Public },
	{ "setScale(float,float)", &slot_2, QMetaData::Public },
	{ "loadImage(const QImage&)", &slot_3, QMetaData::Public },
	{ "loadImage(const QPixmap&)", &slot_4, QMetaData::Public },
	{ "loadImage(const QImage&,GrayImage&)", &slot_5, QMetaData::Public },
	{ "loadImage(const QPixmap&,GrayImage&)", &slot_6, QMetaData::Public },
	{ "loadImage(const QString&)", &slot_7, QMetaData::Public },
	{ "loadTestImage()", &slot_8, QMetaData::Public },
	{ "display()", &slot_9, QMetaData::Public },
	{ "setSelectRadius(int)", &slot_10, QMetaData::Public },
	{ "setSelectRadius(int,int)", &slot_11, QMetaData::Public },
	{ "setSelectColor(QColor&)", &slot_12, QMetaData::Public },
	{ "selectImgRegion(int,int)", &slot_13, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"leftClickOnPixel", 2, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"rightClickOnPixel", 2, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"leftDragOnPixel", 2, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"rightDragOnPixel", 2, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"mouseMovedOnPixel", 2, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_ptr, "QRgb", QUParameter::In }
    };
    static const QUMethod signal_5 = {"mouseMovedOnPixel", 3, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_ptr, "QRgb", QUParameter::In },
	{ 0, &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod signal_6 = {"mouseMovedOnPixel", 4, param_signal_6 };
    static const QUParameter param_signal_7[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_7 = {"adjustSizeNeeded", 2, param_signal_7 };
    static const QUParameter param_signal_8[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_8 = {"selectRadiusChanged", 1, param_signal_8 };
    static const QMetaData signal_tbl[] = {
	{ "leftClickOnPixel(int,int)", &signal_0, QMetaData::Public },
	{ "rightClickOnPixel(int,int)", &signal_1, QMetaData::Public },
	{ "leftDragOnPixel(int,int)", &signal_2, QMetaData::Public },
	{ "rightDragOnPixel(int,int)", &signal_3, QMetaData::Public },
	{ "mouseMovedOnPixel(int,int)", &signal_4, QMetaData::Public },
	{ "mouseMovedOnPixel(int,int,QRgb)", &signal_5, QMetaData::Public },
	{ "mouseMovedOnPixel(int,int,QRgb,float)", &signal_6, QMetaData::Public },
	{ "adjustSizeNeeded(int,int)", &signal_7, QMetaData::Public },
	{ "selectRadiusChanged(int)", &signal_8, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"QtResizeImg", parentObject,
	slot_tbl, 14,
	signal_tbl, 9,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QtResizeImg.setMetaObject( metaObj );
    return metaObj;
}

void* QtResizeImg::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QtResizeImg" ) )
	return this;
    return QWidget::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL leftClickOnPixel
void QtResizeImg::leftClickOnPixel( int t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL rightClickOnPixel
void QtResizeImg::rightClickOnPixel( int t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL leftDragOnPixel
void QtResizeImg::leftDragOnPixel( int t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL rightDragOnPixel
void QtResizeImg::rightDragOnPixel( int t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 3 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL mouseMovedOnPixel
void QtResizeImg::mouseMovedOnPixel( int t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 4 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL mouseMovedOnPixel
void QtResizeImg::mouseMovedOnPixel( int t0, int t1, QRgb t2 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 5 );
    if ( !clist )
	return;
    QUObject o[4];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    static_QUType_ptr.set(o+3,&t2);
    activate_signal( clist, o );
}

// SIGNAL mouseMovedOnPixel
void QtResizeImg::mouseMovedOnPixel( int t0, int t1, QRgb t2, float t3 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 6 );
    if ( !clist )
	return;
    QUObject o[5];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    static_QUType_ptr.set(o+3,&t2);
    static_QUType_ptr.set(o+4,&t3);
    activate_signal( clist, o );
}

// SIGNAL adjustSizeNeeded
void QtResizeImg::adjustSizeNeeded( int t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 7 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL selectRadiusChanged
void QtResizeImg::selectRadiusChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 8, t0 );
}

bool QtResizeImg::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setSize((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 1: setSize((QSize)(*((QSize*)static_QUType_ptr.get(_o+1)))); break;
    case 2: setScale((float)(*((float*)static_QUType_ptr.get(_o+1))),(float)(*((float*)static_QUType_ptr.get(_o+2)))); break;
    case 3: loadImage((const QImage&)*((const QImage*)static_QUType_ptr.get(_o+1))); break;
    case 4: loadImage((const QPixmap&)*((const QPixmap*)static_QUType_ptr.get(_o+1))); break;
    case 5: loadImage((const QImage&)*((const QImage*)static_QUType_ptr.get(_o+1)),(GrayImage&)*((GrayImage*)static_QUType_ptr.get(_o+2))); break;
    case 6: loadImage((const QPixmap&)*((const QPixmap*)static_QUType_ptr.get(_o+1)),(GrayImage&)*((GrayImage*)static_QUType_ptr.get(_o+2))); break;
    case 7: loadImage((const QString&)static_QUType_QString.get(_o+1)); break;
    case 8: loadTestImage(); break;
    case 9: display(); break;
    case 10: setSelectRadius((int)static_QUType_int.get(_o+1)); break;
    case 11: setSelectRadius((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 12: setSelectColor((QColor&)*((QColor*)static_QUType_ptr.get(_o+1))); break;
    case 13: selectImgRegion((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool QtResizeImg::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: leftClickOnPixel((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 1: rightClickOnPixel((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 2: leftDragOnPixel((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 3: rightDragOnPixel((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 4: mouseMovedOnPixel((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 5: mouseMovedOnPixel((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(QRgb)(*((QRgb*)static_QUType_ptr.get(_o+3)))); break;
    case 6: mouseMovedOnPixel((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(QRgb)(*((QRgb*)static_QUType_ptr.get(_o+3))),(float)(*((float*)static_QUType_ptr.get(_o+4)))); break;
    case 7: adjustSizeNeeded((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 8: selectRadiusChanged((int)static_QUType_int.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool QtResizeImg::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool QtResizeImg::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
