#ifndef PLCTOPOLOGYLOADER_EXPORT_H
#define PLCTOPOLOGYLOADER_EXPORT_H

    #include <QtCore/qglobal.h>

    #ifndef PLCTOPOLOGYLOADER_EXPORT
        #if defined(PLCTOPOLOGYMODEL_MAKEDLL)
            /* We are building this library */
            #define PLCTOPOLOGYLOADER_EXPORT Q_DECL_EXPORT

        #else
            /* We are using this library */
            #define PLCTOPOLOGYLOADER_EXPORT Q_DECL_IMPORT

        # endif
    #endif

#endif // PLCTOPOLOGYLOADER_EXPORT_H
