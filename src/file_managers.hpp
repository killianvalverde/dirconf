/* dirconf
 * Copyright (C) 2018-2023 Killian Valverde.
 *
 * This file is part of dirconf.
 *
 * dirconf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dirconf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dirconf. If not, see <http://www.gnu.org/licenses/>.
 */

//
// Created by Killian on 15/07/18.
//

#ifndef DIRCONF_FILE_MANAGERS_HPP
#define DIRCONF_FILE_MANAGERS_HPP


namespace dirconf {


enum class file_managers
{
    NIL = 0x0,
    NEMO = 0x1,
    NAUTILUS = 0x2,
    DOLPHIN = 0x4,
    ALL = 0x7,
};


}


#endif
