/****************************************************************************
** Meta object code from reading C++ file 'shiptablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../shiptablemodel.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'shiptablemodel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSShipTableModelENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSShipTableModelENDCLASS = QtMocHelpers::stringData(
    "ShipTableModel",
    "messageTypesLoaded",
    "",
    "trackNavStatusesLoaded",
    "onNetworkReply",
    "QNetworkReply*",
    "reply",
    "getShipDataForRow",
    "row",
    "getTrackNavStatus",
    "statusId"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSShipTableModelENDCLASS_t {
    uint offsetsAndSizes[22];
    char stringdata0[15];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[23];
    char stringdata4[15];
    char stringdata5[15];
    char stringdata6[6];
    char stringdata7[18];
    char stringdata8[4];
    char stringdata9[18];
    char stringdata10[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSShipTableModelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSShipTableModelENDCLASS_t qt_meta_stringdata_CLASSShipTableModelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "ShipTableModel"
        QT_MOC_LITERAL(15, 18),  // "messageTypesLoaded"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 22),  // "trackNavStatusesLoaded"
        QT_MOC_LITERAL(58, 14),  // "onNetworkReply"
        QT_MOC_LITERAL(73, 14),  // "QNetworkReply*"
        QT_MOC_LITERAL(88, 5),  // "reply"
        QT_MOC_LITERAL(94, 17),  // "getShipDataForRow"
        QT_MOC_LITERAL(112, 3),  // "row"
        QT_MOC_LITERAL(116, 17),  // "getTrackNavStatus"
        QT_MOC_LITERAL(134, 8)   // "statusId"
    },
    "ShipTableModel",
    "messageTypesLoaded",
    "",
    "trackNavStatusesLoaded",
    "onNetworkReply",
    "QNetworkReply*",
    "reply",
    "getShipDataForRow",
    "row",
    "getTrackNavStatus",
    "statusId"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSShipTableModelENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   44,    2, 0x06,    1 /* Public */,
       3,    0,   45,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,   46,    2, 0x08,    3 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       7,    1,   49,    2, 0x102,    5 /* Public | MethodIsConst  */,
       9,    1,   52,    2, 0x102,    7 /* Public | MethodIsConst  */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,

 // methods: parameters
    QMetaType::QJsonObject, QMetaType::Int,    8,
    QMetaType::QString, QMetaType::Int,   10,

       0        // eod
};

Q_CONSTINIT const QMetaObject ShipTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_CLASSShipTableModelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSShipTableModelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSShipTableModelENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ShipTableModel, std::true_type>,
        // method 'messageTypesLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'trackNavStatusesLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNetworkReply'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'getShipDataForRow'
        QtPrivate::TypeAndForceComplete<QJsonObject, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'getTrackNavStatus'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void ShipTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ShipTableModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->messageTypesLoaded(); break;
        case 1: _t->trackNavStatusesLoaded(); break;
        case 2: _t->onNetworkReply((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 3: { QJsonObject _r = _t->getShipDataForRow((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QJsonObject*>(_a[0]) = std::move(_r); }  break;
        case 4: { QString _r = _t->getTrackNavStatus((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 2:
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
            using _t = void (ShipTableModel::*)();
            if (_t _q_method = &ShipTableModel::messageTypesLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ShipTableModel::*)();
            if (_t _q_method = &ShipTableModel::trackNavStatusesLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *ShipTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ShipTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSShipTableModelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int ShipTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ShipTableModel::messageTypesLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ShipTableModel::trackNavStatusesLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
