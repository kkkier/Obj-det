/****************************************************************************
** Meta object code from reading C++ file 'qtimgbrowser.hh'
**
** Created: Tue Feb 5 11:28:04 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qtimgbrowser.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtimgbrowser.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtImgBrowser[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   14,   13,   13, 0x05,
      44,   36,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      90,   70,   13,   13, 0x0a,
     125,  117,   13,   13, 0x2a,
     175,  147,   13,   13, 0x0a,
     239,  223,   13,   13, 0x2a,
     282,   14,   13,   13, 0x0a,
     301,  295,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtImgBrowser[] = {
    "QtImgBrowser\0\0idx\0imageClicked(int)\0"
    "idx,x,y\0imageClicked(int,int,int)\0"
    "vImages,bDispCoords\0load(vector<QImage>&,bool)\0"
    "vImages\0load(vector<QImage>&)\0"
    "vImages,vValues,bDispCoords\0"
    "load(vector<QImage>&,vector<OpGrayImage>&,bool)\0"
    "vImages,vValues\0"
    "load(vector<QImage>&,vector<OpGrayImage>&)\0"
    "display(int)\0px,py\0clickOnImage(int,int)\0"
};

void QtImgBrowser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtImgBrowser *_t = static_cast<QtImgBrowser *>(_o);
        switch (_id) {
        case 0: _t->imageClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->imageClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->load((*reinterpret_cast< vector<QImage>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->load((*reinterpret_cast< vector<QImage>(*)>(_a[1]))); break;
        case 4: _t->load((*reinterpret_cast< vector<QImage>(*)>(_a[1])),(*reinterpret_cast< vector<OpGrayImage>(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 5: _t->load((*reinterpret_cast< vector<QImage>(*)>(_a[1])),(*reinterpret_cast< vector<OpGrayImage>(*)>(_a[2]))); break;
        case 6: _t->display((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->clickOnImage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QtImgBrowser::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtImgBrowser::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtImgBrowser,
      qt_meta_data_QtImgBrowser, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtImgBrowser::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtImgBrowser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtImgBrowser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtImgBrowser))
        return static_cast<void*>(const_cast< QtImgBrowser*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtImgBrowser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void QtImgBrowser::imageClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtImgBrowser::imageClicked(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
