/****************************************************************************
** Meta object code from reading C++ file 'Avaspec.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Avaspec.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Avaspec.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Avaspec_t {
    QByteArrayData data[5];
    char stringdata[34];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Avaspec_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Avaspec_t qt_meta_stringdata_Avaspec = {
    {
QT_MOC_LITERAL(0, 0, 7),
QT_MOC_LITERAL(1, 8, 7),
QT_MOC_LITERAL(2, 16, 0),
QT_MOC_LITERAL(3, 17, 6),
QT_MOC_LITERAL(4, 24, 9)
    },
    "Avaspec\0gotData\0\0newInt\0checkSpec"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Avaspec[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    1,   30,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Avaspec::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Avaspec *_t = static_cast<Avaspec *>(_o);
        switch (_id) {
        case 0: _t->gotData(); break;
        case 1: _t->newInt((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->checkSpec(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Avaspec::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Avaspec::gotData)) {
                *result = 0;
            }
        }
        {
            typedef void (Avaspec::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Avaspec::newInt)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject Avaspec::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Avaspec.data,
      qt_meta_data_Avaspec,  qt_static_metacall, 0, 0}
};


const QMetaObject *Avaspec::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Avaspec::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Avaspec.stringdata))
        return static_cast<void*>(const_cast< Avaspec*>(this));
    return QObject::qt_metacast(_clname);
}

int Avaspec::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Avaspec::gotData()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Avaspec::newInt(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE