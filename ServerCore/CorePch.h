#pragma once

#include "Types.h"
#include "CoreMacro.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"
#include "Container.h"
#include "Memory.h"

#include <iostream>

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <assert.h>
#include <windows.h>

using namespace std;

#include "Lock.h"
#include "SocketUtils.h"
#include "Session.h"
#include "ObjectPool.h"


#pragma comment(lib, "ws2_32.lib")

