/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 * Copyright (C) 2009-2011 MaNGOSZero <https://github.com/mangos/zero>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef DO_POSTGRESQL

#ifndef _DATABASEMYSQL_H
#define _DATABASEMYSQL_H

#include "Common.h"
#include "Database.h"
#include "Policies/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Guard_T.h"

#ifdef WIN32
#include <winsock2.h>
#include <mysql/mysql.h>
#else
#include <mysql.h>
#endif

class MANGOS_DLL_SPEC MySQLConnection : public SqlConnection
{
    public:
        MySQLConnection() : mMysql(NULL) {}
        ~MySQLConnection();

        bool Initialize(const char *infoString);

        QueryResult* Query(const char *sql);
        QueryNamedResult* QueryNamed(const char *sql);
        bool Execute(const char *sql);

        unsigned long escape_string(char *to, const char *from, unsigned long length);

        bool BeginTransaction();
        bool CommitTransaction();
        bool RollbackTransaction();

    private:
        bool _TransactionCmd(const char *sql);
        bool _Query(const char *sql, MYSQL_RES **pResult, MYSQL_FIELD **pFields, uint64* pRowCount, uint32* pFieldCount);

        MYSQL *mMysql;
};

class MANGOS_DLL_SPEC DatabaseMysql : public Database
{
    friend class MaNGOS::OperatorNew<DatabaseMysql>;

    public:
        DatabaseMysql();
        ~DatabaseMysql();

        //! Initializes Mysql and connects to a server.
        /*! infoString should be formated like hostname;username;password;database. */

        // must be call before first query in thread
        void ThreadStart();
        // must be call before finish thread run
        void ThreadEnd();

    protected:
        virtual SqlConnection * CreateConnection();

    private:
        static size_t db_count;
};

#endif
#endif
