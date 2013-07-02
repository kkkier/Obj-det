/****************************************************************************
** QtClusterView meta object code from reading C++ file 'qtclusterview.hh'
**
** Created: Tue Jan 11 17:58:11 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "qtclusterview.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *QtClusterView::className() const
{
    return "QtClusterView";
}

QMetaObject *QtClusterView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QtClusterView( "QtClusterView", &QtClusterView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QtClusterView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtClusterView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QtClusterView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtClusterView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QtClusterView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QtIconView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "nImgId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"displayAssociatedImgs", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "displayAssociatedImgs(int)", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"QtClusterView", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QtClusterView.setMetaObject( metaObj );
    return metaObj;
}

void* QtClusterView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QtClusterView" ) )
	return this;
    return QtIconView::qt_cast( clname );
}

bool QtClusterView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: displayAssociatedImgs((int)static_QUType_int.get(_o+1)); break;
    default:
	return QtIconView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool QtClusterView::qt_emit( int _id, QUObject* _o )
{
    return QtIconView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool QtClusterView::qt_property( int id, int f, QVariant* v)
{
    return QtIconView::qt_property( id, f, v);
}

bool QtClusterView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
