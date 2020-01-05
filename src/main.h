#ifndef MAIN_H
#define MAIN_H

#include <sp2/window.h>
#include <sp2/io/keybinding.h>

extern sp::P<sp::Window> window;
extern sp::io::Keybinding escape_key;

void openMainMenu();

#endif//MAIN_H
