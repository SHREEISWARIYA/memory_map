/****************************************************************************
** Meta object code from reading C++ file 'pasthistory.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../pasthistory.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pasthistory.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSPastHistoryENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSPastHistoryENDCLASS = QtMocHelpers::stringData(
    "PastHistory",
    "trackHistoryFetched",
    "",
    "shipUuid",
    "onNetworkReply",
    "QNetworkReply*",
    "reply",
    "fetchTrackHistory",
    "hours",
    "getTrackHistory"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSPastHistoryENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[12];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[15];
    char stringdata5[15];
    char stringdata6[6];
    char stringdata7[18];
    char stringdata8[6];
    char stringdata9[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSPastHistoryENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSPastHistoryENDCLASS_t qt_meta_stringdata_CLASSPastHistoryENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "PastHistory"
        QT_MOC_LITERAL(12, 19),  // "trackHistoryFetched"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 8),  // "shipUuid"
        QT_MOC_LITERAL(42, 14),  // "onNetworkReply"
        QT_MOC_LITERAL(57, 14),  // "QNetworkReply*"
        QT_MOC_LITERAL(72, 5),  // "reply"
        QT_MOC_LITERAL(78, 17),  // "fetchTrackHistory"
        QT_MOC_LITERAL(96, 5),  // "hours"
        QT_MOC_LITERAL(102, 15)   // "getTrackHistory"
    },
    "PastHistory",
    "trackHistoryFetched",
    "",
    "shipUuid",
    "onNetworkReply",
    "QNetworkReply*",
    "reply",
    "fetchTrackHistory",
    "hours",
    "getTrackHistory"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPastHistoryENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,   41,    2, 0x08,    3 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       7,    2,   44,    2, 0x02,    5 /* Public */,
       9,    1,   49,    2, 0x102,    8 /* Public | MethodIsConst  */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    8,
    QMetaType::QVariantList, QMetaType::QString,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject PastHistory::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSPastHistoryENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPastHistoryENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPastHistoryENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PastHistory, std::true_type>,
        // method 'trackHistoryFetched'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onNetworkReply'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'fetchTrackHistory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'getTrackHistory'
        QtPrivate::TypeAndForceComplete<QVariantList, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void PastHistory::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PastHistory *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->trackHistoryFetched((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->onNetworkReply((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 2: _t->fetchTrackHistory((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: { QVariantList _r = _t->getTrackHistory((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
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
            using _t = void (PastHistory::*)(const QString & );
            if (_t _q_method = &PastHistory::trackHistoryFetched; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *PastHistory::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PastHistory::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPastHistoryENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PastHistory::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    }
    return _id;
}

// SIGNAL 0
void PastHistory::trackHistoryFetched(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
