/*
    This file is part of Soundbench.

    Soundbench is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Soundbench is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Soundbench.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012  Amaya S.
*/


#ifndef PROBLEMFIX_H
#define PROBLEMFIX_H

namespace sb {

    namespace errs {
        class ProblemFix {
        public:
            ProblemFix() {
                fixed = false;
            }
            virtual const char* desc() {return "";}
            virtual const char* comments() {return "";}
            virtual ~ProblemFix() {}
            virtual bool fixit() {
                fixed = true;
                return fixed;
            }
            bool wasFixed() {
                return fixed;
            }
        private:
            bool fixed;
        };
    }
}

#endif // PROBLEMFIX_H
