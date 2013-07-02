/****************************************************************************
** QtImgBrowser meta object code from reading C++ file 'qtimgbrowser.hh'
**
** Created: Tue Jan 11 17:58:03 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "qtimgbrowser.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *QtImgBrowser::className() const
{
    return "QtImgBrowser";
}

QMetaObject *QtImgBrowser::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QtImgBrowser( "QtImgBrowser", &QtImgBrowser::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QtImgBrowser::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtImgBrowser", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QtImgBrowser::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtImgBrowser", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QtImgBrowser::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "vImages", &static_QUType_ptr, "vector<QImage>", QUParameter::InOut }
    };
    static const QUMethod slot_0 = {"load", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "vImages", &static_QUType_ptr, "vector<QImage>", QUParameter::InOut },
	{ "bDispCoords", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"load", 2, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "vImages", &static_QUType_ptr, "vector<QImage>", QUParameter::InOut },
	{ "vValues", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut }
    };
    static const QUMethod slot_2 = {"load", 2, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "vImages", &static_QUType_ptr, "vector<QImage>", QUParameter::InOut },
	{ "vValues", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "bDispCoords", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"load", 3, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "idx", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"display", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "px", &static_QUType_int, 0, QUParameter::In },
	{ "py", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"clickOnImage", 2, param_slot_5 };
    static const QMetaData slot_tbl[] = {
	{ "load(vector<QImage>&)", &slot_0, QMetaData::Public },
	{ "load(vector<QImage>&,bool)", &slot_1, QMetaData::Public },
	{ "load(vector<QImage>&,vector<OpGrayImage>&)", &slot_2, QMetaData::Public },
	{ "load(vector<QImage>&,vector<OpGrayImage>&,bool)", &slot_3, QMetaData::Public },
	{ "display(int)", &slot_4, QMetaData::Public },
	{ "clickOnImage(int,int)", &slot_5, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ "idx", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"imageClicked", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "idx", &static_QUType_int, 0, QUParameter::In },
	{ "x", &static_QUType_int, 0, QUParameter::In },
	{ "y", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"imageClicked", 3, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "imageClicked(int)", &signal_0, QMetaData::Public },
	{ "imageClicked(int,int,int)", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"QtImgBrowser", parentObject,
	slot_tbl, 6,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QtImgBrowser.setMetaObject( metaObj );
    return metaObj;
}

void* QtImgBrowser::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QtImgBrowser" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL imageClicked
void QtImgBrowser::imageClicked( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL imageClicked
void QtImgBrowser::imageClicked( int t0, int t1, int t2 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[4];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    static_QUType_int.set(o+3,t2);
    activate_signal( clist, o );
}

bool QtImgBrowser::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: load((vector<QImage>&)*((vector<QImage>*)static_QUType_ptr.get(_o+1))); break;
    case 1: load((vector<QImage>&)*((vector<QImage>*)static_QUType_ptr.get(_o+1)),(bool)static_QUType_bool.get(_o+2)); break;
    case 2: load((vector<QImage>&)*((vector<QImage>*)static_QUType_ptr.get(_o+1)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+2))); break;
    case 3: load((vector<QImage>&)*((vector<QImage>*)static_QUType_ptr.get(_o+1)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+2)),(bool)static_QUType_bool.get(_o+3)); break;
    case 4: display((int)static_QUType_int.get(_o+1)); break;
    case 5: clickOnImage((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool QtImgBrowser::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: imageClicked((int)static_QUType_int.get(_o+1)); break;
    case 1: imageClicked((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool QtImgBrowser::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool QtImgBrowser::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
