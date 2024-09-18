/****************************************************************************
** Meta object code from reading C++ file 'pagination.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../pagination.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pagination.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSPaginationENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSPaginationENDCLASS = QtMocHelpers::stringData(
    "Pagination",
    "shipsChanged",
    "",
    "onNetworkReply",
    "QNetworkReply*",
    "reply",
    "ships",
    "QMap<QString,QVariantMap>",
    "printShipDetails",
    "uuid"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSPaginationENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[11];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[15];
    char stringdata5[6];
    char stringdata6[6];
    char stringdata7[26];
    char stringdata8[17];
    char stringdata9[5];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSPaginationENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSPaginationENDCLASS_t qt_meta_stringdata_CLASSPaginationENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "Pagination"
        QT_MOC_LITERAL(11, 12),  // "shipsChanged"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 14),  // "onNetworkReply"
        QT_MOC_LITERAL(40, 14),  // "QNetworkReply*"
        QT_MOC_LITERAL(55, 5),  // "reply"
        QT_MOC_LITERAL(61, 5),  // "ships"
        QT_MOC_LITERAL(67, 25),  // "QMap<QString,QVariantMap>"
        QT_MOC_LITERAL(93, 16),  // "printShipDetails"
        QT_MOC_LITERAL(110, 4)   // "uuid"
    },
    "Pagination",
    "shipsChanged",
    "",
    "onNetworkReply",
    "QNetworkReply*",
    "reply",
    "ships",
    "QMap<QString,QVariantMap>",
    "printShipDetails",
    "uuid"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPaginationENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       1,   46, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   38,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    1,   39,    2, 0x08,    3 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,   42,    2, 0x102,    5 /* Public | MethodIsConst  */,
       8,    1,   43,    2, 0x102,    6 /* Public | MethodIsConst  */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    5,

 // methods: parameters
    0x80000000 | 7,
    QMetaType::Void, QMetaType::QString,    9,

 // properties: name, type, flags
       6, 0x80000000 | 7, 0x00015009, uint(0), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject Pagination::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSPaginationENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPaginationENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPaginationENDCLASS_t,
        // property 'ships'
        QtPrivate::TypeAndForceComplete<QMap<QString,QVariantMap>, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Pagination, std::true_type>,
        // method 'shipsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNetworkReply'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'ships'
        QtPrivate::TypeAndForceComplete<QMap<QString,QVariantMap>, std::false_type>,
        // method 'printShipDetails'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void Pagination::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Pagination *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->shipsChanged(); break;
        case 1: _t->onNetworkReply((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 2: { QMap<QString,QVariantMap> _r = _t->ships();
            if (_a[0]) *reinterpret_cast< QMap<QString,QVariantMap>*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->printShipDetails((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Pagination::*)();
            if (_t _q_method = &Pagination::shipsChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    } else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<Pagination *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QMap<QString,QVariantMap>*>(_v) = _t->ships(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *Pagination::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Pagination::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPaginationENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Pagination::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Pagination::shipsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
