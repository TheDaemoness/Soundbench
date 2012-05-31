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
        namespace fixes {
            class ProblemFix {
            public:
                ProblemFix() {
                    comments = "";
                    desc = "";
                    fixed = false;
                }
                virtual const char* getDesc() {return desc.c_str();}
                virtual const char* getComments() {return comments.c_str();}
                virtual ~ProblemFix() {}
                virtual bool runFix() {
                    fixed = theFix();
                    return fixed;
                }
                bool wasFixed() {
                    return fixed;
                }
            protected:
                virtual bool theFix() = 0;
                std::string comments;
                std::string desc;
                bool fixed;
            };

            //The following should only be used with ErrorPopups
            class Ignore : public ProblemFix {
            public:
                Ignore() {
                    comments = "Ignore the problem and continue.";
                    desc = "Ignore/Continue";
                }
                void setComments(std::string comm) {
                    comments = comm;
                }
                virtual bool theFix() {
                    return true;
                }

            };

        }
    }
}

#endif // PROBLEMFIX_H
