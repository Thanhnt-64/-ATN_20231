/*
 * Copyright (C) 2022 Yago Fontoura do Rosario <yago.rosario@hotmail.com.br>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * \addtogroup gecko-platforms
 * @{
 *
 * \file
 *  Configuration for the gecko platform
 */
#ifndef CONTIKI_CONF_H
#define CONTIKI_CONF_H

#include <stdint.h>
#include <inttypes.h>
/*---------------------------------------------------------------------------*/
/* Include Project Specific conf */
#ifdef PROJECT_CONF_PATH
#include PROJECT_CONF_PATH
#endif /* PROJECT_CONF_PATH */
/*---------------------------------------------------------------------------*/
#include "gecko-def.h"
/*---------------------------------------------------------------------------*/
/* Include Project Specific conf */
#define PLATFORM_HEADER "mgm24-conf.h"
#include "mgm24-conf.h"
// #ifdef BOARD_CONF_PATH
// #include BOARD_CONF_PATH
// #else /* BOARD_CONF_PATH */
// #error "BOARD_CONF_PATH undefined"
// #endif /* BOARD_CONF_PATH */
/*---------------------------------------------------------------------------*/
/* Include CPU Group configuration */
#include "gecko-conf.h"
/*---------------------------------------------------------------------------*/

// #define CC_CONF_VA_ARGS                1
#ifndef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE		140
#endif

#ifndef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE		140
#endif

#define PROCESS_CONF_NUMEVENTS       8
#define PROCESS_CONF_STATS           0



/*---------------Netstack mac layer-----------------------------------------------------------------------------*/

#define MAC_CONF_WITH_CSMA 1

        /* Framer selection. The framer is used by the MAC implementation
         to build and parse frames. */

#define NETSTACK_CONF_FRAMER    framer_802154




/*---------------Netstack network layer-----------------------------------------------------------------------------*/

/* Routing protocol configuration. The Routing protocol is configured through the Makefile,
   via the flag MAKE_ROUTING */



#define ROUTING_CONF_RPL_LITE   1


#define NETSTACK_CONF_WITH_IPV6 1



/*-------------------------------------------------------------------------------------------------------------------*/



/* configure network size and density */
#ifndef NETSTACK_MAX_ROUTE_ENTRIES
#define NETSTACK_MAX_ROUTE_ENTRIES   6
#endif /* NETSTACK_MAX_ROUTE_ENTRIES */
#ifndef NBR_TABLE_CONF_MAX_NEIGHBORS
#define NBR_TABLE_CONF_MAX_NEIGHBORS 6
#endif /* NBR_TABLE_CONF_MAX_NEIGHBORS */

/* configure queues */
// #ifndef QUEUEBUF_CONF_NUM
// #define QUEUEBUF_CONF_NUM 64
// #endif /* QUEUEBUF_CONF_NUM */

#define UIP_CONF_IPV6_QUEUE_PKT  1
#define UIP_ARCH_IPCHKSUM        1





/* TSCH and RPL callbacks */



#include <ctype.h>


#define LOG_CONF_ENABLED 1



#endif /* CONTIKI_CONF_H */

/** @} */
