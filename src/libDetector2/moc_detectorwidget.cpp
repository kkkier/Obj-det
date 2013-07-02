/****************************************************************************
** DetectorWidget meta object code from reading C++ file 'detectorwidget.hh'
**
** Created: Tue Dec 25 06:30:05 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "detectorwidget.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *DetectorWidget::className() const
{
    return "DetectorWidget";
}

QMetaObject *DetectorWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_DetectorWidget( "DetectorWidget", &DetectorWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString DetectorWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DetectorWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString DetectorWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DetectorWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* DetectorWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"propagateCueUpdate", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"propagateDetectorUpdate", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "e", &static_QUType_ptr, "QHideEvent", QUParameter::In }
    };
    static const QUMethod slot_2 = {"hideEvent", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "e", &static_QUType_ptr, "QCloseEvent", QUParameter::In }
    };
    static const QUMethod slot_3 = {"closeEvent", 1, param_slot_3 };
    static const QUMethod slot_4 = {"loadDetector", 0, 0 };
    static const QUMethod slot_5 = {"saveDetector", 0, 0 };
    static const QUMethod slot_6 = {"clearDetector", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "propagateCueUpdate(const QString&)", &slot_0, QMetaData::Public },
	{ "propagateDetectorUpdate(const QString&)", &slot_1, QMetaData::Public },
	{ "hideEvent(QHideEvent*)", &slot_2, QMetaData::Public },
	{ "closeEvent(QCloseEvent*)", &slot_3, QMetaData::Public },
	{ "loadDetector()", &slot_4, QMetaData::Public },
	{ "saveDetector()", &slot_5, QMetaData::Public },
	{ "clearDetector()", &slot_6, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"sigCuesChanged", 0, 0 };
    static const QUMethod signal_1 = {"sigDetectorChanged", 0, 0 };
    static const QUMethod signal_2 = {"sigLoadDetectorClicked", 0, 0 };
    static const QUMethod signal_3 = {"sigSaveDetectorClicked", 0, 0 };
    static const QUMethod signal_4 = {"sigClearDetectorClicked", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "sigCuesChanged()", &signal_0, QMetaData::Public },
	{ "sigDetectorChanged()", &signal_1, QMetaData::Public },
	{ "sigLoadDetectorClicked()", &signal_2, QMetaData::Public },
	{ "sigSaveDetectorClicked()", &signal_3, QMetaData::Public },
	{ "sigClearDetectorClicked()", &signal_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"DetectorWidget", parentObject,
	slot_tbl, 7,
	signal_tbl, 5,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_DetectorWidget.setMetaObject( metaObj );
    return metaObj;
}

void* DetectorWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "DetectorWidget" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL sigCuesChanged
void DetectorWidget::sigCuesChanged()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL sigDetectorChanged
void DetectorWidget::sigDetectorChanged()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

// SIGNAL sigLoadDetectorClicked
void DetectorWidget::sigLoadDetectorClicked()
{
    activate_signal( staticMetaObject()->signalOffset() + 2 );
}

// SIGNAL sigSaveDetectorClicked
void DetectorWidget::sigSaveDetectorClicked()
{
    activate_signal( staticMetaObject()->signalOffset() + 3 );
}

// SIGNAL sigClearDetectorClicked
void DetectorWidget::sigClearDetectorClicked()
{
    activate_signal( staticMetaObject()->signalOffset() + 4 );
}

bool DetectorWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: propagateCueUpdate((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: propagateDetectorUpdate((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: hideEvent((QHideEvent*)static_QUType_ptr.get(_o+1)); break;
    case 3: closeEvent((QCloseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 4: loadDetector(); break;
    case 5: saveDetector(); break;
    case 6: clearDetector(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool DetectorWidget::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigCuesChanged(); break;
    case 1: sigDetectorChanged(); break;
    case 2: sigLoadDetectorClicked(); break;
    case 3: sigSaveDetectorClicked(); break;
    case 4: sigClearDetectorClicked(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool DetectorWidget::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool DetectorWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
