//////////////////////////////////////////////////////////////////////
// This file is part of Remere's Map Editor
//////////////////////////////////////////////////////////////////////
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////

#include "main.h"
#include "main_toolbar.h"
#include "gui.h"
#include "editor.h"
#include "settings.h"
#include "brush.h"
#include "pngfiles.h"

#include <wx/artprov.h>
#include <wx/mstream.h>

const wxString MainToolBar::STANDARD_BAR_NAME = "standard_toolbar";
const wxString MainToolBar::BRUSHES_BAR_NAME = "brushes_toolbar";
const wxString MainToolBar::POSITION_BAR_NAME = "position_toolbar";

#define loadPNGFile(name) _wxGetBitmapFromMemory(name, sizeof(name))
inline wxBitmap* _wxGetBitmapFromMemory(const unsigned char* data, int length)
{
	wxMemoryInputStream is(data, length);
	wxImage img(is, "image/png");
	if (!img.IsOk()) return nullptr;
	return newd wxBitmap(img, -1);
}

MainToolBar::MainToolBar(wxWindow* parent, wxAuiManager* manager)
{
	wxSize icon_size = wxSize(16, 16);
	wxBitmap new_bitmap = wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR, icon_size);
	wxBitmap open_bitmap = wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, icon_size);
	wxBitmap save_bitmap = wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR, icon_size);
	wxBitmap saveas_bitmap = wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR, icon_size);
	wxBitmap undo_bitmap = wxArtProvider::GetBitmap(wxART_UNDO, wxART_TOOLBAR, icon_size);
	wxBitmap redo_bitmap = wxArtProvider::GetBitmap(wxART_REDO, wxART_TOOLBAR, icon_size);
	wxBitmap cut_bitmap = wxArtProvider::GetBitmap(wxART_CUT, wxART_TOOLBAR, icon_size);
	wxBitmap copy_bitmap = wxArtProvider::GetBitmap(wxART_COPY, wxART_TOOLBAR, icon_size);
	wxBitmap paste_bitmap = wxArtProvider::GetBitmap(wxART_PASTE, wxART_TOOLBAR, icon_size);

	standard_toolbar = newd wxAuiToolBar(parent, TOOLBAR_STANDARD, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE);
	standard_toolbar->SetToolBitmapSize(icon_size);
	standard_toolbar->AddTool(wxID_NEW, wxEmptyString, new_bitmap, wxNullBitmap, wxITEM_NORMAL, "New Map", wxEmptyString, NULL);
	standard_toolbar->AddTool(wxID_OPEN, wxEmptyString, open_bitmap, wxNullBitmap, wxITEM_NORMAL, "Open Map", wxEmptyString, NULL);
	standard_toolbar->AddTool(wxID_SAVE, wxEmptyString, save_bitmap, wxNullBitmap, wxITEM_NORMAL, "Save Map", wxEmptyString, NULL);
	standard_toolbar->AddTool(wxID_SAVEAS, wxEmptyString, saveas_bitmap, wxNullBitmap, wxITEM_NORMAL, "Save Map As...", wxEmptyString, NULL);
	standard_toolbar->AddSeparator();
	standard_toolbar->AddTool(wxID_UNDO, wxEmptyString, undo_bitmap, wxNullBitmap, wxITEM_NORMAL, "Undo", wxEmptyString, NULL);
	standard_toolbar->AddTool(wxID_REDO, wxEmptyString, redo_bitmap, wxNullBitmap, wxITEM_NORMAL, "Redo", wxEmptyString, NULL);
	standard_toolbar->AddSeparator();
	standard_toolbar->AddTool(wxID_CUT, wxEmptyString, cut_bitmap, wxNullBitmap, wxITEM_NORMAL, "Cut", wxEmptyString, NULL);
	standard_toolbar->AddTool(wxID_COPY, wxEmptyString, copy_bitmap, wxNullBitmap, wxITEM_NORMAL, "Copy", wxEmptyString, NULL);
	standard_toolbar->AddTool(wxID_PASTE, wxEmptyString, paste_bitmap, wxNullBitmap, wxITEM_NORMAL, "Paste", wxEmptyString, NULL);
	standard_toolbar->Realize();

	wxBitmap* border_bitmap = loadPNGFile(optional_border_small_png);
	wxBitmap* eraser_bitmap = loadPNGFile(eraser_small_png);
	wxBitmap* pz_bitmap = loadPNGFile(protection_zone_small_png);
	wxBitmap* nopvp_bitmap = loadPNGFile(no_pvp_small_png);
	wxBitmap* nologout_bitmap = loadPNGFile(no_logout_small_png);
	wxBitmap* pvp_bitmap = loadPNGFile(pvp_zone_small_png);
	wxBitmap* normal_bitmap = loadPNGFile(door_normal_small_png);
	wxBitmap* locked_bitmap = loadPNGFile(door_locked_small_png);
	wxBitmap* magic_bitmap = loadPNGFile(door_magic_small_png);
	wxBitmap* quest_bitmap = loadPNGFile(door_quest_small_png);
	wxBitmap* hatch_bitmap = loadPNGFile(window_hatch_small_png);
	wxBitmap* window_bitmap = loadPNGFile(window_normal_small_png);

	brushes_toolbar = newd wxAuiToolBar(parent, TOOLBAR_BRUSHES, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE);
	brushes_toolbar->SetToolBitmapSize(icon_size);
	brushes_toolbar->AddTool(PALETTE_TERRAIN_OPTIONAL_BORDER_TOOL, wxEmptyString, *border_bitmap, wxNullBitmap, wxITEM_CHECK, "Border", wxEmptyString, NULL);
	brushes_toolbar->AddTool(PALETTE_TERRAIN_ERASER, wxEmptyString, *eraser_bitmap, wxNullBitmap, wxITEM_CHECK, "Eraser", wxEmptyString, NULL);
	brushes_toolbar->AddSeparator();
	brushes_toolbar->AddTool(PALETTE_TERRAIN_PZ_TOOL, wxEmptyString, *pz_bitmap, wxNullBitmap, wxITEM_CHECK, "Protected Zone", wxEmptyString, NULL);
	brushes_toolbar->AddTool(PALETTE_TERRAIN_NOPVP_TOOL, wxEmptyString, *nopvp_bitmap, wxNullBitmap, wxITEM_CHECK, "No PvP Zone", wxEmptyString, NULL);
	brushes_toolbar->AddTool(PALETTE_TERRAIN_NOLOGOUT_TOOL, wxEmptyString, *nologout_bitmap, wxNullBitmap, wxITEM_CHECK, "No Logout Zone", wxEmptyString, NULL);
	brushes_toolbar->AddTool(PALETTE_TERRAIN_PVPZONE_TOOL, wxEmptyString, *pvp_bitmap, wxNullBitmap, wxITEM_CHECK, "PvP Zone", wxEmptyString, NULL);
	brushes_toolbar->AddSeparator();
	brushes_toolbar->AddTool(PALETTE_TERRAIN_NORMAL_DOOR, wxEmptyString, *normal_bitmap, wxNullBitmap, wxITEM_CHECK, "Normal Door", wxEmptyString, NULL);
	brushes_toolbar->AddTool(PALETTE_TERRAIN_LOCKED_DOOR, wxEmptyString, *locked_bitmap, wxNullBitmap, wxITEM_CHECK, "Locked Door", wxEmptyString, NULL);
	brushes_toolbar->AddTool(PALETTE_TERRAIN_MAGIC_DOOR, wxEmptyString, *magic_bitmap, wxNullBitmap, wxITEM_CHECK, "Magic Door", wxEmptyString, NULL);
	brushes_toolbar->AddTool(PALETTE_TERRAIN_QUEST_DOOR, wxEmptyString, *quest_bitmap, wxNullBitmap, wxITEM_CHECK, "Quest Door", wxEmptyString, NULL);
	brushes_toolbar->AddTool(PALETTE_TERRAIN_HATCH_DOOR, wxEmptyString, *hatch_bitmap, wxNullBitmap, wxITEM_CHECK, "Hatch Window", wxEmptyString, NULL);
	brushes_toolbar->AddTool(PALETTE_TERRAIN_WINDOW_DOOR, wxEmptyString, *window_bitmap, wxNullBitmap, wxITEM_CHECK, "Window", wxEmptyString, NULL);
	brushes_toolbar->Realize();

	wxBitmap go_bitmap = wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR, icon_size);

	position_toolbar = newd wxAuiToolBar(parent, TOOLBAR_POSITION, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORZ_TEXT);
	position_toolbar->SetToolBitmapSize(icon_size);
	x_control = newd NumberTextCtrl(position_toolbar, wxID_ANY, 0, 0, MAP_MAX_WIDTH, wxTE_PROCESS_ENTER, "X", wxDefaultPosition, wxSize(60, 20));
	x_control->SetToolTip("X Coordinate");
	y_control = newd NumberTextCtrl(position_toolbar, wxID_ANY, 0, 0, MAP_MAX_HEIGHT, wxTE_PROCESS_ENTER, "Y", wxDefaultPosition, wxSize(60, 20));
	y_control->SetToolTip("Y Coordinate");
	z_control = newd NumberTextCtrl(position_toolbar, wxID_ANY, 0, 0, MAP_MAX_LAYER, wxTE_PROCESS_ENTER, "Z", wxDefaultPosition, wxSize(35, 20));
	z_control->SetToolTip("Z Coordinate");
	position_toolbar->AddControl(x_control);
	position_toolbar->AddControl(y_control);
	position_toolbar->AddControl(z_control);
	position_toolbar->AddTool(TOOLBAR_POSITION_GO, wxEmptyString, go_bitmap, wxNullBitmap, wxITEM_NORMAL, "Go To Position", wxEmptyString, NULL);
	position_toolbar->Realize();

	manager->AddPane(standard_toolbar, wxAuiPaneInfo().Name(STANDARD_BAR_NAME).ToolbarPane().Top().Row(1).Position(1).Floatable(false));
	manager->AddPane(brushes_toolbar, wxAuiPaneInfo().Name(BRUSHES_BAR_NAME).ToolbarPane().Top().Row(1).Position(2).Floatable(false));
	manager->AddPane(position_toolbar, wxAuiPaneInfo().Name(POSITION_BAR_NAME).ToolbarPane().Top().Row(1).Position(3).Floatable(false));

	standard_toolbar->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainToolBar::OnStandardButtonClick, this);
	brushes_toolbar->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainToolBar::OnBrushesButtonClick, this);
	position_toolbar->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainToolBar::OnPositionButtonClick, this);
	x_control->Bind(wxEVT_TEXT_PASTE, &MainToolBar::OnPastePositionText, this);
	y_control->Bind(wxEVT_TEXT_PASTE, &MainToolBar::OnPastePositionText, this);
	z_control->Bind(wxEVT_TEXT_PASTE, &MainToolBar::OnPastePositionText, this);

	HideAll();
}

MainToolBar::~MainToolBar()
{
	standard_toolbar->Unbind(wxEVT_COMMAND_MENU_SELECTED, &MainToolBar::OnStandardButtonClick, this);
	brushes_toolbar->Unbind(wxEVT_COMMAND_MENU_SELECTED, &MainToolBar::OnBrushesButtonClick, this);
	position_toolbar->Unbind(wxEVT_COMMAND_MENU_SELECTED, &MainToolBar::OnPositionButtonClick, this);
	x_control->Unbind(wxEVT_TEXT_PASTE, &MainToolBar::OnPastePositionText, this);
	y_control->Unbind(wxEVT_TEXT_PASTE, &MainToolBar::OnPastePositionText, this);
	z_control->Unbind(wxEVT_TEXT_PASTE, &MainToolBar::OnPastePositionText, this);
}

void MainToolBar::UpdateButtons()
{
	Editor* editor = g_gui.GetCurrentEditor();
	if (editor) {
		standard_toolbar->EnableTool(wxID_UNDO, editor->actionQueue->canUndo());
		standard_toolbar->EnableTool(wxID_REDO, editor->actionQueue->canRedo());
		standard_toolbar->EnableTool(wxID_PASTE, editor->copybuffer.canPaste());
	} else {
		standard_toolbar->EnableTool(wxID_UNDO, false);
		standard_toolbar->EnableTool(wxID_REDO, false);
		standard_toolbar->EnableTool(wxID_PASTE, false);
	}

	bool has_map = editor != nullptr;
	bool is_host = has_map && !editor->IsLiveClient();

	standard_toolbar->EnableTool(wxID_SAVE, is_host);
	standard_toolbar->EnableTool(wxID_SAVEAS, is_host);
	standard_toolbar->EnableTool(wxID_CUT, has_map);
	standard_toolbar->EnableTool(wxID_COPY, has_map);

	brushes_toolbar->EnableTool(PALETTE_TERRAIN_OPTIONAL_BORDER_TOOL, has_map);
	brushes_toolbar->EnableTool(PALETTE_TERRAIN_ERASER, has_map);
	brushes_toolbar->EnableTool(PALETTE_TERRAIN_PZ_TOOL, has_map);
	brushes_toolbar->EnableTool(PALETTE_TERRAIN_NOPVP_TOOL, has_map);
	brushes_toolbar->EnableTool(PALETTE_TERRAIN_NOLOGOUT_TOOL, has_map);
	brushes_toolbar->EnableTool(PALETTE_TERRAIN_PVPZONE_TOOL, has_map);
	brushes_toolbar->EnableTool(PALETTE_TERRAIN_NORMAL_DOOR, has_map);
	brushes_toolbar->EnableTool(PALETTE_TERRAIN_LOCKED_DOOR, has_map);
	brushes_toolbar->EnableTool(PALETTE_TERRAIN_MAGIC_DOOR, has_map);
	brushes_toolbar->EnableTool(PALETTE_TERRAIN_QUEST_DOOR, has_map);
	brushes_toolbar->EnableTool(PALETTE_TERRAIN_HATCH_DOOR, has_map);
	brushes_toolbar->EnableTool(PALETTE_TERRAIN_WINDOW_DOOR, has_map);

	position_toolbar->EnableTool(TOOLBAR_POSITION_GO, has_map);
	x_control->Enable(has_map);
	y_control->Enable(has_map);
	z_control->Enable(has_map);
}

void MainToolBar::UpdateBrushButtons()
{
	Brush* brush = g_gui.GetCurrentBrush();
	if (brush) {
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_OPTIONAL_BORDER_TOOL, brush == g_gui.optional_brush);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_ERASER, brush == g_gui.eraser);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_PZ_TOOL, brush == g_gui.pz_brush);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_NOPVP_TOOL, brush == g_gui.rook_brush);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_NOLOGOUT_TOOL, brush == g_gui.nolog_brush);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_PVPZONE_TOOL, brush == g_gui.pvp_brush);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_NORMAL_DOOR, brush == g_gui.normal_door_brush);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_LOCKED_DOOR, brush == g_gui.locked_door_brush);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_MAGIC_DOOR, brush == g_gui.magic_door_brush);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_QUEST_DOOR, brush == g_gui.quest_door_brush);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_HATCH_DOOR, brush == g_gui.hatch_door_brush);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_WINDOW_DOOR, brush == g_gui.window_door_brush);
	} else {
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_OPTIONAL_BORDER_TOOL, false);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_ERASER, false);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_PZ_TOOL, false);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_NOPVP_TOOL, false);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_NOLOGOUT_TOOL, false);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_PVPZONE_TOOL, false);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_NORMAL_DOOR, false);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_LOCKED_DOOR, false);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_MAGIC_DOOR, false);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_QUEST_DOOR, false);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_HATCH_DOOR, false);
		brushes_toolbar->ToggleTool(PALETTE_TERRAIN_WINDOW_DOOR, false);
	}
	g_gui.GetAuiManager()->Update();
}

void MainToolBar::Show(ToolBarID id, bool show)
{
	wxAuiManager* manager = g_gui.GetAuiManager();
	if (manager) {
		wxAuiPaneInfo& pane = GetPane(id);
		if (pane.IsOk()) {
			pane.Show(show);
			manager->Update();
		}
	}
}

void MainToolBar::HideAll(bool update)
{
	wxAuiManager* manager = g_gui.GetAuiManager();
	if (!manager)
		return;

	wxAuiPaneInfoArray& panes = manager->GetAllPanes();
	for (int i = 0, count = panes.GetCount(); i < count; ++i) {
		if (!panes.Item(i).IsToolbar())
			panes.Item(i).Hide();
	}

	if (update)
		manager->Update();
}

void MainToolBar::LoadPerspective()
{
	wxAuiManager* manager = g_gui.GetAuiManager();
	if (!manager)
		return;

	if (g_settings.getBoolean(Config::SHOW_TOOLBAR_STANDARD)) {
		std::string info = g_settings.getString(Config::TOOLBAR_STANDARD_LAYOUT);
		if (!info.empty())
			manager->LoadPaneInfo(wxString(info), GetPane(TOOLBAR_STANDARD));
		GetPane(TOOLBAR_STANDARD).Show();
	} else
		GetPane(TOOLBAR_STANDARD).Hide();
		
	if (g_settings.getBoolean(Config::SHOW_TOOLBAR_BRUSHES)) {
		std::string info = g_settings.getString(Config::TOOLBAR_BRUSHES_LAYOUT);
		if (!info.empty())
			manager->LoadPaneInfo(wxString(info), GetPane(TOOLBAR_BRUSHES));
		GetPane(TOOLBAR_BRUSHES).Show();
	} else
		GetPane(TOOLBAR_BRUSHES).Hide();

	if (g_settings.getBoolean(Config::SHOW_TOOLBAR_POSITION)) {
		std::string info = g_settings.getString(Config::TOOLBAR_POSITION_LAYOUT);
		if (!info.empty())
			manager->LoadPaneInfo(wxString(info), GetPane(TOOLBAR_POSITION));
		GetPane(TOOLBAR_POSITION).Show();
	} else
		GetPane(TOOLBAR_POSITION).Hide();

	manager->Update();
}

void MainToolBar::SavePerspective()
{
	wxAuiManager* manager = g_gui.GetAuiManager();
	if (!manager)
		return;

	if (g_settings.getBoolean(Config::SHOW_TOOLBAR_STANDARD)) {
		wxString info = manager->SavePaneInfo(GetPane(TOOLBAR_STANDARD));
		g_settings.setString(Config::TOOLBAR_STANDARD_LAYOUT, info.ToStdString());
	}

	if (g_settings.getBoolean(Config::SHOW_TOOLBAR_BRUSHES)) {
		wxString info = manager->SavePaneInfo(GetPane(TOOLBAR_BRUSHES));
		g_settings.setString(Config::TOOLBAR_BRUSHES_LAYOUT, info.ToStdString());
	}

	if (g_settings.getBoolean(Config::SHOW_TOOLBAR_POSITION)) {
		wxString info = manager->SavePaneInfo(GetPane(TOOLBAR_POSITION));
		g_settings.setString(Config::TOOLBAR_POSITION_LAYOUT, info.ToStdString());
	}
}

void MainToolBar::OnStandardButtonClick(wxCommandEvent& event)
{
	switch (event.GetId()) {
		case wxID_NEW:
			g_gui.NewMap();
			break;
		case wxID_OPEN:
			g_gui.OpenMap();
			break;
		case wxID_SAVE:
			g_gui.SaveMap();
			break;
		case wxID_SAVEAS:
			g_gui.SaveMapAs();
			break;
		case wxID_UNDO:
			g_gui.DoUndo();
			break;
		case wxID_REDO:
			g_gui.DoRedo();
			break;
		case wxID_CUT:
			g_gui.DoCut();
			break;
		case wxID_COPY:
			g_gui.DoCopy();
			break;
		case wxID_PASTE:
			g_gui.PreparePaste();
			break;
		default:
			break;
	}
}

void MainToolBar::OnBrushesButtonClick(wxCommandEvent& event)
{
	if (!g_gui.IsEditorOpen())
		return;

	switch (event.GetId()) {
		case PALETTE_TERRAIN_OPTIONAL_BORDER_TOOL:
			g_gui.SelectBrush(g_gui.optional_brush);
			break;
		case PALETTE_TERRAIN_ERASER:
			g_gui.SelectBrush(g_gui.eraser);
			break;
		case PALETTE_TERRAIN_PZ_TOOL:
			g_gui.SelectBrush(g_gui.pz_brush);
			break;
		case PALETTE_TERRAIN_NOPVP_TOOL:
			g_gui.SelectBrush(g_gui.rook_brush);
			break;
		case PALETTE_TERRAIN_NOLOGOUT_TOOL:
			g_gui.SelectBrush(g_gui.nolog_brush);
			break;
		case PALETTE_TERRAIN_PVPZONE_TOOL:
			g_gui.SelectBrush(g_gui.pvp_brush);
			break;
		case PALETTE_TERRAIN_NORMAL_DOOR:
			g_gui.SelectBrush(g_gui.normal_door_brush);
			break;
		case PALETTE_TERRAIN_LOCKED_DOOR:
			g_gui.SelectBrush(g_gui.locked_door_brush);
			break;
		case PALETTE_TERRAIN_MAGIC_DOOR:
			g_gui.SelectBrush(g_gui.magic_door_brush);
			break;
		case PALETTE_TERRAIN_QUEST_DOOR:
			g_gui.SelectBrush(g_gui.quest_door_brush);
			break;
		case PALETTE_TERRAIN_HATCH_DOOR:
			g_gui.SelectBrush(g_gui.hatch_door_brush);
			break;
		case PALETTE_TERRAIN_WINDOW_DOOR:
			g_gui.SelectBrush(g_gui.window_door_brush);
			break;
		default:
			break;
	}
}

void MainToolBar::OnPositionButtonClick(wxCommandEvent& event)
{
	if (!g_gui.IsEditorOpen())
		return;

	if (event.GetId() == TOOLBAR_POSITION_GO) {
		Position pos(x_control->GetIntValue(), y_control->GetIntValue(), z_control->GetIntValue());
		if (pos.x != 0 && pos.y != 0 && pos.z != 0)
			g_gui.SetScreenCenterPosition(pos);
	}
}

void MainToolBar::OnPastePositionText(wxClipboardTextEvent& event)
{
	Position position;
	if (posFromClipboard(position.x, position.y, position.z)) {
		x_control->SetIntValue(position.x);
		y_control->SetIntValue(position.y);
		z_control->SetIntValue(position.z);
	} else
		event.Skip();
}

wxAuiPaneInfo& MainToolBar::GetPane(ToolBarID id)
{
	wxAuiManager* manager = g_gui.GetAuiManager();
	if (!manager)
		return wxAuiNullPaneInfo;

	switch (id) {
		case TOOLBAR_STANDARD:
			return manager->GetPane(STANDARD_BAR_NAME);
		case TOOLBAR_BRUSHES:
			return manager->GetPane(BRUSHES_BAR_NAME);
		case TOOLBAR_POSITION:
			return manager->GetPane(POSITION_BAR_NAME);
		default:
			return wxAuiNullPaneInfo;
	}
}