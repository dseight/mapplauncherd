/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of applauncherd
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef BOOSTER_H
#define BOOSTER_H

#include <cstdlib>
#include <string>

using std::string;

#include "appdata.h"

/*!
 *  \class Booster
 *  \brief Abstract base class for all boosters (Qt-booster, M-booster and so on..)
 *
 *  Booster is a class that is used to initialize certain resources in libraries
 *  common to all applications of a type.
 *
 *  Booster also communicates with the invoker process and handles the actual
 *  jump to the main() -method of the application to be launched.
 *
 *  Booster instance dies with the launched application and a new one must be created
 *  in advance so as to launch a new application.
 */
class Booster
{
public:

    //! Constructor
    Booster();

    //! Destructor
    virtual ~Booster();

    /*!
     * \brief Run the application to be invoked.
     * This method causes the application binary to be loaded
     * using dlopen(). Program execution jumps to the address of
     * "main()" found in the newly loaded library. The Booster process
     * exits with corresponding exit-code after the execution of
     * main() has finished.
     */
    void run();

    /*!
     * \brief Wait for connection from invoker and read the input.
     * This method accepts a socket connection from the invoker
     * and reads the data of an application to be launched.
     *
     * \return true on success
     */
    bool readCommand();

    /*!
     * \brief Initialize and preload stuff
     * Override in the custom Booster.
     */
    virtual bool preload();

    /*!
     * \brief Rename process.
     * This method overrides the argument data starting from initialArgv[0].
     * This is needed so as to get the process name and arguments displayed
     * correctly e.g. in the listing by 'ps'. initialArgv[1] may provide an
     * empty dummy space to be used. It is assumed, that the arguments are
     * located continuosly in memory and this is how it's done in glibc.
     *
     * \param initialArgc Number of the arguments of the launcher process.
     * \param initialArgv Address of the argument array of the launcher process.
     */
    void renameProcess(int initialArgc, char** initialArgv);

    /*!
     * \brief Return booster type common to all instances.
     * This is used in the simple communication between booster process.
     * and the daemon. Override in the custom Booster.
     *
     * \return A (unique) character representing the type of the Booster.
     */
    virtual char boosterType() const = 0;

    //! Set nice value and store the old priority. Return true on success.
    bool pushPriority(int nice);

    //! Restore the old priority stored by the previous successful setPriority().
    bool popPriority();

protected:

    /*!
     * \brief Return the communication socket used by a Booster.
     * This method returns the socket used between invoker and the Booster.
     * (common to all Boosters of the type). Override in the custom Booster.
     * \return Path to the socket file
     */
    virtual const string & socketId() const = 0;

private:

    //! Disable copy-constructor
    Booster(const Booster & r);

    //! Disable assignment operator
    Booster & operator= (const Booster & r);


    void  complainAndExit();
    void  launchProcess();
    void* loadMain();
    AppData m_app;

    int m_argvArraySize;
    int m_oldPriority;
    bool m_oldPriorityOk;

#ifdef UNIT_TEST
    friend class Ut_Booster;
#endif
};

#endif // BOOSTER_H
