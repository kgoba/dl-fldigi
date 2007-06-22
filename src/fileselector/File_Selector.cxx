//
// "$Id: File_Selector.cxx 4723 2005-12-30 10:13:17Z matt $"
//
// File_Selector dialog for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

// generated by Fast Light User Interface Designer (fluid) version 1.0107

#include "File_Selector.h"
#include <FL/fl_draw.H>

void File_Selector::cb_window_i(Fl_Double_Window*, void*) {
  fileName->value("");
fileList->deselect();
Fl::remove_timeout((Fl_Timeout_Handler)previewCB, this);
window->hide();
}
void File_Selector::cb_window(Fl_Double_Window* o, void* v) {
  ((File_Selector*)(o->user_data()))->cb_window_i(o,v);
}

void File_Selector::cb_showChoice_i(Fl_Choice*, void*) {
  showChoiceCB();
}
void File_Selector::cb_showChoice(Fl_Choice* o, void* v) {
  ((File_Selector*)(o->parent()->parent()->user_data()))->cb_showChoice_i(o,v);
}

void File_Selector::cb_newButton_i(Fl_Button*, void*) {
  newdir();
}
void File_Selector::cb_newButton(Fl_Button* o, void* v) {
  ((File_Selector*)(o->parent()->parent()->user_data()))->cb_newButton_i(o,v);
}

#include <FL/Fl_Bitmap.H>
static unsigned char idata_new[] =
{0,0,120,0,132,0,2,1,1,254,1,128,49,128,49,128,253,128,253,128,49,128,49,
128,1,128,1,128,255,255,0,0};
static Fl_Bitmap image_new(idata_new, 16, 16);

void File_Selector::cb__i(Fl_Tile*, void*) {
  update_preview();
}
void File_Selector::cb_(Fl_Tile* o, void* v) {
  ((File_Selector*)(o->parent()->user_data()))->cb__i(o,v);
}

void File_Selector::cb_fileList_i(Fl_File_Browser*, void*) {
  fileListCB();
}
void File_Selector::cb_fileList(Fl_File_Browser* o, void* v) {
  ((File_Selector*)(o->parent()->parent()->user_data()))->cb_fileList_i(o,v);
}

void File_Selector::cb_previewButton_i(Fl_Check_Button*, void*) {
  preview(previewButton->value());
}
void File_Selector::cb_previewButton(Fl_Check_Button* o, void* v) {
  ((File_Selector*)(o->parent()->parent()->parent()->user_data()))->cb_previewButton_i(o,v);
}

void File_Selector::cb_fileName_i(Fl_File_Input*, void*) {
  fileNameCB();
}
void File_Selector::cb_fileName(Fl_File_Input* o, void* v) {
  ((File_Selector*)(o->parent()->parent()->user_data()))->cb_fileName_i(o,v);
}

void File_Selector::cb_okButton_i(Fl_Return_Button*, void*) {
  // Do any callback that is registered...
if (callback_)
  (*callback_)(this, data_);

window->hide();
}
void File_Selector::cb_okButton(Fl_Return_Button* o, void* v) {
  ((File_Selector*)(o->parent()->parent()->parent()->user_data()))->cb_okButton_i(o,v);
}

void File_Selector::cb_cancelButton_i(Fl_Button*, void*) {
  fileName->value("");
fileList->deselect();
Fl::remove_timeout((Fl_Timeout_Handler)previewCB, this);
window->hide();
}
void File_Selector::cb_cancelButton(Fl_Button* o, void* v) {
  ((File_Selector*)(o->parent()->parent()->parent()->user_data()))->cb_cancelButton_i(o,v);
}

File_Selector::File_Selector(const char *d, const char *p, int t, const char *title) {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = window = new Fl_Double_Window(490, 380, "Choose File");
    w = o;
    o->callback((Fl_Callback*)cb_window, (void*)(this));
    { Fl_Group* o = new Fl_Group(10, 10, 470, 25);
      { Fl_Choice* o = showChoice = new Fl_Choice(65, 10, 215, 25, "Show:");
        o->down_box(FL_BORDER_BOX);
        o->labelfont(1);
        o->callback((Fl_Callback*)cb_showChoice);
        Fl_Group::current()->resizable(o);
        showChoice->label(show_label);
      }
      { Fl_Button* o = newButton = new Fl_Button(455, 10, 25, 25);
        o->image(image_new);
        o->labelsize(8);
        o->callback((Fl_Callback*)cb_newButton);
        o->tooltip(new_directory_tooltip);
      }
      o->end();
    }
    { Fl_Tile* o = new Fl_Tile(10, 45, 470, 225);
      o->callback((Fl_Callback*)cb_);
      { Fl_File_Browser* o = fileList = new Fl_File_Browser(10, 45, 295, 225);
        o->type(2);
        o->callback((Fl_Callback*)cb_fileList);
        w->hotspot(o);
      }
      { Fl_Box* o = previewBox = new Fl_Box(305, 45, 175, 225, "?");
        o->box(FL_DOWN_BOX);
        o->labelsize(100);
        o->align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE);
      }
      o->end();
      Fl_Group::current()->resizable(o);
    }
    { Fl_Group* o = new Fl_Group(10, 275, 470, 95);
      { Fl_Group* o = new Fl_Group(10, 275, 470, 20);
        { Fl_Check_Button* o = previewButton = new Fl_Check_Button(10, 275, 73, 20, "Preview");
          o->down_box(FL_DOWN_BOX);
          o->value(1);
          o->shortcut(0x80070);
          o->callback((Fl_Callback*)cb_previewButton);
          previewButton->label(preview_label);
        }
        { Fl_Box* o = new Fl_Box(115, 275, 365, 20);
          Fl_Group::current()->resizable(o);
        }
        o->end();
      }
      { Fl_File_Input* o = fileName = new Fl_File_Input(115, 300, 365, 35);
        o->labelfont(1);
        o->callback((Fl_Callback*)cb_fileName);
        o->when(FL_WHEN_ENTER_KEY);
        Fl_Group::current()->resizable(o);
        fileName->when(FL_WHEN_CHANGED | FL_WHEN_ENTER_KEY_ALWAYS);
      }
      { Fl_Box* o = new Fl_Box(10, 310, 105, 25, "Filename:");
        o->labelfont(1);
        o->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
        o->label(filename_label);
      }
      { Fl_Group* o = new Fl_Group(10, 345, 470, 25);
        { Fl_Return_Button* o = okButton = new Fl_Return_Button(313, 345, 85, 25, "OK");
          o->callback((Fl_Callback*)cb_okButton);
          okButton->label(fl_ok);
        }
        { Fl_Button* o = cancelButton = new Fl_Button(408, 345, 72, 25, "Cancel");
          o->callback((Fl_Callback*)cb_cancelButton);
          o->label(fl_cancel);
        }
        { Fl_Box* o = new Fl_Box(10, 345, 30, 25);
          Fl_Group::current()->resizable(o);
        }
        o->end();
      }
      o->end();
    }
    if (title) window->label(title);
    o->set_modal();
    o->end();
  }
data_ = 0;
directory_[0] = 0;
window->size_range(window->w(), window->h(), Fl::w(), Fl::h());
type(t);
filter(p);
value(d);
type(t);
preview(1);
}

File_Selector::~File_Selector() {
  Fl::remove_timeout((Fl_Timeout_Handler)previewCB, this);
delete window;
}

void File_Selector::callback(void (*cb)(File_Selector *, void *), void *d ) {
  callback_ = cb;
data_     = d;
}

void File_Selector::color(Fl_Color c) {
  fileList->color(c);
}

Fl_Color File_Selector::color() {
  return (fileList->color());
}

char * File_Selector::directory() {
  return directory_;
}

const char * File_Selector::filter() {
  return (fileList->filter());
}

int File_Selector::filter_value() {
  return showChoice->value();
}

void File_Selector::filter_value(int f) {
  showChoice->value(f);
showChoiceCB();
}

void File_Selector::hide() {
  window->hide();
}

void File_Selector::iconsize(uchar s) {
  fileList->iconsize(s);
}

uchar File_Selector::iconsize() {
  return (fileList->iconsize());
}

void File_Selector::label(const char *l) {
  window->label(l);
}

const char * File_Selector::label() {
  return (window->label());
}

void File_Selector::ok_label(const char *l) {
  okButton->label(l);
int w=0, h=0;
okButton->measure_label(w, h);
okButton->resize(cancelButton->x() - 50 - w, cancelButton->y(),
                 w + 40, 25);
okButton->parent()->init_sizes();
}

const char * File_Selector::ok_label() {
  return (okButton->label());
}

void File_Selector::show() {
  window->hotspot(fileList);
window->show();
Fl::flush();
fl_cursor(FL_CURSOR_WAIT);
rescan();
fl_cursor(FL_CURSOR_DEFAULT);
fileName->take_focus();
}

int File_Selector::shown() {
  return window->shown();
}

void File_Selector::textcolor(Fl_Color c) {
  fileList->textcolor(c);
}

Fl_Color File_Selector::textcolor() {
  return (fileList->textcolor());
}

void File_Selector::textfont(uchar f) {
  fileList->textfont(f);
}

uchar File_Selector::textfont() {
  return (fileList->textfont());
}

void File_Selector::textsize(uchar s) {
  fileList->textsize(s);
}

uchar File_Selector::textsize() {
  return (fileList->textsize());
}

void File_Selector::type(int t) {
  type_ = t;
if (t & MULTI)
  fileList->type(FL_MULTI_BROWSER);
else
  fileList->type(FL_HOLD_BROWSER);
if (t & CREATE)
  newButton->activate();
else
  newButton->deactivate();
if (t & DIRECTORY)
  fileList->filetype(Fl_File_Browser::DIRECTORIES);
else
  fileList->filetype(Fl_File_Browser::FILES);
}

int File_Selector::type() {
  return (type_);
}

void * File_Selector::user_data() const {
  return (data_);
}

void File_Selector::user_data(void *d) {
  data_ = d;
}

int File_Selector::visible() {
  return window->visible();
}

//
// End of "$Id: File_Selector.cxx 4723 2005-12-30 10:13:17Z matt $".
//
