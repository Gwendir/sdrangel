///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#include <QtPlugin>
#include <QAction>
#include "plugin/pluginapi.h"

#include "nfmmodgui.h"
#include "nfmmodplugin.h"

const PluginDescriptor NFMModPlugin::m_pluginDescriptor = {
    QString("NFM Modulator"),
    QString("2.5.2"),
    QString("(c) Edouard Griffiths, F4EXB"),
    QString("https://github.com/f4exb/sdrangel"),
    true,
    QString("https://github.com/f4exb/sdrangel")
};

NFMModPlugin::NFMModPlugin(QObject* parent) :
    QObject(parent),
    m_pluginAPI(0)
{
}

const PluginDescriptor& NFMModPlugin::getPluginDescriptor() const
{
    return m_pluginDescriptor;
}

void NFMModPlugin::initPlugin(PluginAPI* pluginAPI)
{
	m_pluginAPI = pluginAPI;

	// register AM modulator
	m_pluginAPI->registerTxChannel(NFMModGUI::m_channelID, this);
}

PluginGUI* NFMModPlugin::createTxChannel(const QString& channelName, DeviceSinkAPI *deviceAPI)
{
	if(channelName == NFMModGUI::m_channelID)
	{
	    NFMModGUI* gui = NFMModGUI::create(m_pluginAPI, deviceAPI);
		return gui;
	} else {
		return 0;
	}
}

void NFMModPlugin::createInstanceModNFM(DeviceSinkAPI *deviceAPI)
{
    NFMModGUI* gui = NFMModGUI::create(m_pluginAPI, deviceAPI);
}
