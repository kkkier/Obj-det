/****************************************************************************
** Meta object code from reading C++ file 'qticonview.hh'
**
** Created: Tue Feb 5 11:28:03 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qticonview.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qticonview.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtIconView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   42,   11,   11, 0x0a,
      99,   84,   11,   11, 0x0a,
     139,   84,   11,   11, 0x0a,
     180,   84,   11,   11, 0x0a,
     230,  225,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtIconView[] = {
    "QtIconView\0\0nImgId\0mouseClickOnImage(int)\0"
    "img,sKey\0addImageThumbnail(QImage,string)\0"
    "img,sText,sKey\0addImageThumbnail(QImage,string,string)\0"
    "addImageThumbnail(QPixmap,string,string)\0"
    "addImageThumbnail(OpGrayImage,string,string)\0"
    "item\0mouseClickOnItem(QListWidgetItem*)\0"
};

void QtIconView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtIconView *_t = static_cast<QtIconView *>(_o);
        switch (_id) {
        case 0: _t->mouseClickOnImage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->addImageThumbnail((*reinterpret_cast< QImage(*)>(_a[1])),(*reinterpret_cast< string(*)>(_a[2]))); break;
        case 2: _t->addImageThumbnail((*reinterpret_cast< QImage(*)>(_a[1])),(*reinterpret_cast< string(*)>(_a[2])),(*reinterpret_cast< string(*)>(_a[3]))); break;
        case 3: _t->addImageThumbnail((*reinterpret_cast< QPixmap(*)>(_a[1])),(*reinterpret_cast< string(*)>(_a[2])),(*reinterpret_cast< string(*)>(_a[3]))); break;
        case 4: _t->addImageThumbnail((*reinterpret_cast< OpGrayImage(*)>(_a[1])),(*reinterpret_cast< string(*)>(_a[2])),(*reinterpret_cast< string(*)>(_a[3]))); break;
        case 5: _t->mouseClickOnItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QtIconView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtIconView::staticMetaObject = {
    { &QListWidget::staticMetaObject, qt_meta_stringdata_QtIconView,
      qt_meta_data_QtIconView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtIconView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtIconView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtIconView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtIconView))
        return static_cast<void*>(const_cast< QtIconView*>(this));
    return QListWidget::qt_metacast(_clname);
}

int QtIconView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QtIconView::mouseClickOnImage(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
