.. Copyright Epic Games, Inc. All Rights Reserved.

==========
Setup
==========

This setup guide demonstrates how to enable and set up the Deadline plugin.

Enabling the Deadline Plugin
----------------------------

To enable the plugin:

#. Open the plugin browser by selecting **Edit > Plugins** from the main menu.
#. Search for **Epic Deadline Service** in the search dialog.
#. Enable the plugin and restart the Editor.

.. note::
    If you cannot find the plugin, ensure that the plugin files are located under **<EngineVersion>/Engine/Plugins**
    or any custom subdirectory in the **Engine/Plugins** folder. Make sure that the plugin is compiled for the current
    engine version. The Editor will notify you on the next launch if the plugin has not been compiled and will suggest that you compile
    the plugin from source.

    The Deadline service depends on the python **urllib3** library. For legal reasons, this library is not packaged with the plugin.
    If the python interpreter running in the Editor has this library installed, then no further configuration is needed. Otherwise, you will need
    to download *urllib3* via an external source and copy the site packages into the **Content/Python/Lib/<Platform>/** directory in the **UnrealDeadlineService** plugin.
    ("Platform" on a windows operating system is **Win64**). The Editor will automatically load any python libraries in the specified path.

.. image:: _images/unreal_deadline_plugins_browser.png
    :align: center
    :width: 1000px

|

Project Settings
-----------------

To configure the project settings:

#. Open the project settings browser by selecting **Edit > Project Settings** from the main menu.
#. Search for Deadline in the search browser.
#. In the Deadline Host, type the address and port to the webservice. For example, **http://127.0.0.1:8081** for a locally hosted deadline repository webservice listening on port **8081**.

.. note::

    | **Deadline Host**: This setting specifies the webservice address to submit Deadline jobs to. The expected value is **<ip/hostname>:<port>**.
        **This is the most important setting**.

    | **Script Category**: This setting allows you to specify a named directory path. You can create a list of key-value pairs describing directories. Each
        key can be used in the Deadline data asset to reference a directory without explicitly typing it.
        The full path is resolved later in the submission process.

.. image:: _images/unreal_deadline_project_settings.png
    :align: center
    :width: 1000px

|
