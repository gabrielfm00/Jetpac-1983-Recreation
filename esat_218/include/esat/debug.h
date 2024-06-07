/* Copyright      2017 ESAT. All rights reserved.    #####  #####      #     #####
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>       #      #         # #      #
 *                                                   ###     ###     #   #     #  
 * Window API (main graphics display)                #          #   #     #    #
 *                                                   #####  #####  #       #   #
 */

#ifndef ESAT_DEBUG_H
#define ESAT_DEBUG_H 1

namespace esat {

  void DebugMemView(const char *window_title, const void *mem, size_t mem_size ); 

} /* ESAT */

#endif