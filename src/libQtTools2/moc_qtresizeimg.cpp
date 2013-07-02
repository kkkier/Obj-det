/****************************************************************************
** Meta object code from reading C++ file 'qtresizeimg.hh'
**
** Created: Tue Feb 5 11:28:04 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qtresizeimg.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtresizeimg.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtResizeImg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   13,   12,   12, 0x05,
      41,   13,   12,   12, 0x05,
      68,   13,   12,   12, 0x05,
      93,   13,   12,   12, 0x05,
     119,   13,   12,   12, 0x05,
     149,  146,   12,   12, 0x05,
     185,  181,   12,   12, 0x05,
     223,   13,   12,   12, 0x05,
     249,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     278,  274,   12,   12, 0x0a,
     300,  295,   12,   12, 0x0a,
     321,  315,   12,   12, 0x0a,
     347,  343,   12,   12, 0x0a,
     372,  365,   12,   12, 0x0a,
     403,  391,   12,   12, 0x0a,
     447,  432,   12,   12, 0x0a,
     486,  477,   12,   12, 0x0a,
     505,   12,   12,   12, 0x0a,
     521,   12,   12,   12, 0x0a,
     533,  531,   12,   12, 0x0a,
     558,  554,   12,   12, 0x0a,
     589,  583,   12,   12, 0x0a,
     613,  554,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtResizeImg[] = {
    "QtResizeImg\0\0,\0leftClickOnPixel(int,int)\0"
    "rightClickOnPixel(int,int)\0"
    "leftDragOnPixel(int,int)\0"
    "rightDragOnPixel(int,int)\0"
    "mouseMovedOnPixel(int,int)\0,,\0"
    "mouseMovedOnPixel(int,int,QRgb)\0,,,\0"
    "mouseMovedOnPixel(int,int,QRgb,float)\0"
    "adjustSizeNeeded(int,int)\0"
    "selectRadiusChanged(int)\0w,h\0"
    "setSize(int,int)\0size\0setSize(QSize)\0"
    "sx,sy\0setScale(float,float)\0img\0"
    "loadImage(QImage)\0pixmap\0loadImage(QPixmap)\0"
    "img,grayImg\0loadImage(QImage,GrayImage&)\0"
    "pixmap,grayImg\0loadImage(QPixmap,GrayImage&)\0"
    "filename\0loadImage(QString)\0loadTestImage()\0"
    "display()\0r\0setSelectRadius(int)\0x,y\0"
    "setSelectRadius(int,int)\0color\0"
    "setSelectColor(QColor&)\0"
    "selectImgRegion(int,int)\0"
};

void QtResizeImg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtResizeImg *_t = static_cast<QtResizeImg *>(_o);
        switch (_id) {
        case 0: _t->leftClickOnPixel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->rightClickOnPixel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->leftDragOnPixel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->rightDragOnPixel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->mouseMovedOnPixel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->mouseMovedOnPixel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QRgb(*)>(_a[3]))); break;
        case 6: _t->mouseMovedOnPixel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QRgb(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 7: _t->adjustSizeNeeded((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->selectRadiusChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->setSize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->setSize((*reinterpret_cast< QSize(*)>(_a[1]))); break;
        case 11: _t->setScale((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 12: _t->loadImage((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        case 13: _t->loadImage((*reinterpret_cast< const QPixmap(*)>(_a[1]))); break;
        case 14: _t->loadImage((*reinterpret_cast< const QImage(*)>(_a[1])),(*reinterpret_cast< GrayImage(*)>(_a[2]))); break;
        case 15: _t->loadImage((*reinterpret_cast< const QPixmap(*)>(_a[1])),(*reinterpret_cast< GrayImage(*)>(_a[2]))); break;
        case 16: _t->loadImage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->loadTestImage(); break;
        case 18: _t->display(); break;
        case 19: _t->setSelectRadius((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->setSelectRadius((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 21: _t->setSelectColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 22: _t->selectImgRegion((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QtResizeImg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtResizeImg::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtResizeImg,
      qt_meta_data_QtResizeImg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtResizeImg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtResizeImg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtResizeImg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtResizeImg))
        return static_cast<void*>(const_cast< QtResizeImg*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtResizeImg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void QtResizeImg::leftClickOnPixel(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtResizeImg::rightClickOnPixel(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtResizeImg::leftDragOnPixel(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtResizeImg::rightDragOnPixel(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QtResizeImg::mouseMovedOnPixel(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QtResizeImg::mouseMovedOnPixel(int _t1, int _t2, QRgb _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QtResizeImg::mouseMovedOnPixel(int _t1, int _t2, QRgb _t3, float _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QtResizeImg::adjustSizeNeeded(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void QtResizeImg::selectRadiusChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
