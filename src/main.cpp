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

#include <speed/speed.hpp>
#include <speed/speed_alias.hpp>

#include "program.hpp"


int main(int argc, char* argv[])
{
    spd::ap::arg_parser ap("dirconf");
    ap.add_help_text("Apply directories configuration files editing the GNOME Virtual file system "
                     "directories metatada. JSON files are used to specify the configuration in "
                     "the directories. Currently is only implemented for nemo.\n");
    ap.add_key_value_arg({"--configuration-files", "-cf"},
                         "Configuration files names. The default value is '.dirconf.json'.",
                         {spd::ap::avt_t::STRING}, 1u, ~0u);
    ap.add_help_arg({"--help"}, "Display this help and exit.");
    ap.add_gplv3_version_arg({"--version"}, "Output version information and exit", "1.0.0", "2023",
                             "Killian Poulaud");
    ap.add_keyless_arg("DIR", "Directory", "", {spd::ap::avt_t::RWX_DIR});
    ap.add_help_text("");
    ap.add_help_text("Example:\n"
                     "$ dirconf /home/user");
    ap.add_help_text("");
    ap.add_help_text("Example of JSON file:\n"
                     "{\n"
                     "    \"icon-path\" : \"\",\n"
                     "    \"view\": \"icons\",\n"
                     "    \"icon-view-zoom-level\" : 4,\n"
                     "    \"list-view-zoom-level\" : 1,\n"
                     "    \"compact-layout\" : true,\n"
                     "    \"apply-recursively\" : false\n"
                     "}");
    ap.parse_args((unsigned int)argc, argv);
    
    dirconf::program prog(
            ap.get_front_arg_value_as<std::filesystem::path>("DIR"),
            ap.get_arg_values_as<std::string>("--configuration-files", {".dirconf.json"}),
            dirconf::file_managers::NEMO);
    
    return prog.execute();
}
