/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009

	M Roberts (original release)
	Robin Birch <robinb@ruffnready.co.uk>
	Samuel Gisiger <samuel.gisiger@triadis.ch>
	Jeff Goodenough <jeff@enborne.f2s.com>
	Alastair Harrison <aharrison@magic.force9.co.uk>
	Scott Penrose <scottp@dd.com.au>
	John Wharington <jwharington@gmail.com>
	Lars H <lars_hn@hotmail.com>
	Rob Dunning <rob@raspberryridgesheepfarm.com>
	Russell King <rmk@arm.linux.org.uk>
	Paolo Ventafridda <coolwind@email.it>
	Tobias Lohner <tobias@lohner-net.de>
	Mirek Jezek <mjezek@ipplc.cz>
	Max Kellermann <max@duempel.org>
	Tobias Bieniek <tobias.bieniek@gmx.de>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Form/Keyboard.hpp"

#include "StringUtil.hpp"
#include "Screen/Layout.hpp"
#include "Screen/ButtonWindow.hpp"
#include "Form/Form.hpp"
#include "Form/Container.hpp"

static const TCHAR keyboard_letters[] =
  _T("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ");

KeyboardControl::KeyboardControl(WndForm &form, ContainerWindow &parent,
                                 int x, int y, unsigned width, unsigned height,
                                 unsigned button_width, unsigned button_height,
                                 Color background_color, const Font *font,
                                 const WindowStyle _style) :
  background_brush(background_color),
  button_width(button_width), button_height(button_height),
  parent_form(form)
{
  set(parent, x, y, width, height, _style);

  TCHAR caption[] = _T(" ");
  TCHAR name[] = _T("cmd ");

  for (const TCHAR *i = keyboard_letters; !string_is_empty(i); ++i) {
    caption[0] = *i;
    name[3] = *i;

    add_button(form, name, caption, font);
  }

  add_button(form, _T("cmdSpace"), _T(" Space "), font);
  add_button(form, _T("cmdPeriod"), _T("."), font);
  add_button(form, _T("cmdComma"), _T(","), font);
  add_button(form, _T("cmdMinus"), _T("-"), font);

  move_buttons();
}

static void
ApplyAllowedCharacters(Window *window, TCHAR ch, const TCHAR *allowed)
{
  if (window != NULL)
    window->set_visible(allowed == NULL || _tcschr(allowed, ch) != NULL);
}

void
KeyboardControl::SetAllowedCharacters(const TCHAR *allowed)
{
  TCHAR name[] = _T("cmd ");
  for (const TCHAR *i = keyboard_letters; !string_is_empty(i); ++i) {
    name[3] = *i;
    ApplyAllowedCharacters(get_button(name), *i, allowed);
  }

  ApplyAllowedCharacters(get_button(_T("cmdSpace")), ' ', allowed);
  ApplyAllowedCharacters(get_button(_T("cmdPeriod")), '.', allowed);
  ApplyAllowedCharacters(get_button(_T("cmdComma")), ',', allowed);
  ApplyAllowedCharacters(get_button(_T("cmdMinus")), '-', allowed);
}

ButtonWindow *
KeyboardControl::get_button(const TCHAR* name)
{
  return (ButtonWindow *)parent_form.FindByName(name);
}

ButtonWindow *
KeyboardControl::get_button_by_caption(const TCHAR* caption)
{
  if (_tcscmp(caption, _T(",")) == 0)
    return get_button(_T("cmdComma"));
  if (_tcscmp(caption, _T(".")) == 0)
    return get_button(_T("cmdPeriod"));
  if (_tcscmp(caption, _T("-")) == 0)
    return get_button(_T("cmdMinus"));
  if (_tcscmp(caption, _T(" ")) == 0 || _tcscmp(caption, _T(" Space ")) == 0)
    return get_button(_T("cmdSpace"));

  if (_tcslen(caption) == 1 && _tcschr(keyboard_letters, caption[0]) != NULL) {
    TCHAR name[5] = _T("cmd");
    _tcsncat(name, caption, 1);
    name[4] = 0;

    return get_button(name);
  }

  return NULL;
}

void
KeyboardControl::move_button(const TCHAR* name, int left, int top)
{
  ButtonWindow *kb = get_button(name);
  if (kb)
    kb->move(left, top);
}

void
KeyboardControl::resize_button(const TCHAR* name,
                               unsigned int width, unsigned int height)
{
  ButtonWindow *kb = get_button(name);
  if (kb)
    kb->resize(width, height);
}

void
KeyboardControl::resize_buttons()
{
  ButtonWindow *kb;
  for (unsigned i = 0; i < _tcslen(keyboard_letters); i++) {
    TCHAR caption[2];
    _tcsncpy(caption, keyboard_letters + i, 1);
    caption[1] = 0;

    kb = get_button_by_caption(caption);
    if (!kb)
      continue;

    kb->resize(button_width, button_height);
  }
  resize_button(_T("cmdComma"), button_width, button_height);
  resize_button(_T("cmdPeriod"), button_width, button_height);
  resize_button(_T("cmdMinus"), button_width, button_height);
  resize_button(_T("cmdSpace"), button_width, button_height);
}

void
KeyboardControl::move_buttons_to_row(const TCHAR* buttons, int row, int offset)
{
  if (string_is_empty(buttons))
    return;

  ButtonWindow *kb;
  for (unsigned i = 0; i < _tcslen(buttons); i++) {
    TCHAR caption[2];
    _tcsncpy(caption, buttons + i, 1);
    caption[1] = 0;

    kb = get_button_by_caption(caption);
    if (!kb)
      continue;

    kb->move(i * button_width + offset, row * button_height);
  }
}

void
KeyboardControl::move_buttons()
{
  move_buttons_to_row(_T("1234567890"), 0);
  move_buttons_to_row(_T("QWERTYUIOP"), 1);
  move_buttons_to_row(_T("ASDFGHJKL"), 2, button_width * 0.333);
  move_buttons_to_row(_T("ZXCVBNM,."), 3, button_width * 0.667);

  if (Layout::landscape) {
    move_button(_T("cmdMinus"), button_width * 9, button_height * 4);

    move_button(_T("cmdSpace"), button_width * 2.5, button_height * 4);
    resize_button(_T("cmdSpace"), button_width * 3, button_height);
  } else {
    move_button(_T("cmdMinus"), button_width * 8, button_height * 4.5);

    move_button(_T("cmdSpace"), button_width * 2, button_height * 4.5);
    resize_button(_T("cmdSpace"), button_width * 5, button_height);
  }                              
}

void
KeyboardControl::on_paint(Canvas &canvas)
{
  canvas.clear(background_brush);
}

bool
KeyboardControl::on_command(unsigned id, unsigned code)
{
  if (id >= 0x20) {
    mOnCharacter((TCHAR)id);
    return true;
  } else
    return ContainerWindow::on_command(id, code);
}

void
KeyboardControl::add_button(WndForm &form, const TCHAR* name,
                            const TCHAR* caption, const Font *font)
{
  ButtonWindow *button = new ButtonWindow();
  button->set(*this, caption, (unsigned)caption[0],
              0, 0, button_width, button_height);
  button->set_font(*font);
  form.AddNamed(name, button);
  form.AddDestruct(button);
}
