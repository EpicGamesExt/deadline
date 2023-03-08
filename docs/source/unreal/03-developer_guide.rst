.. Copyright Epic Games, Inc. All Rights Reserved.

===========
Hello World
===========

In this section, we will demonstrate how to create a custom deadline job submission script using Python.
This custom implementation shows how to create and submit Deadline jobs based on dynamic data using the Deadline Service.

.. note::
    To follow best practices, we recommend creating a Content Plugin in the **<EngineVersion>/Engine/Plugins** directory. This
    plugin enables you to store your Python scripts and execute them without specifying absolute paths. See
    `Scripting the Unreal Editor Using Python <https://docs.unrealengine.com/5.1/en-US/scripting-the-unreal-editor-using-python/>`_
    for more details on using Python in the Editor.

To create the custom deadline job submission script, follow these steps:

#. Create a python file called *hello_world.py* in the **Python** directory of your content plugin.
#. Import the **unreal** python module:

    .. code-block:: python

        import unreal

#. Import the Deadline Service class:

    .. note::
        The Deadline service is a singleton. This service is persisted in interpreter and exposed through a helper function, **get_global_deadline_service_instance**
        in the deadline service module.  This ensures that only a single instance of the Deadline Service is returned.

    .. code-block:: python

        from deadline_service import get_global_deadline_service_instance

#. Import the Deadline job class:

    .. code-block:: python

        from deadline_job import DeadlineJob

#. Import a utility function for getting the job info and plugin info from a Deadline Data Asset:

    .. code-block:: python

        from deadline_utils import get_deadline_info_from_preset


#. Get the path to the Data Asset, load the asset, and get the data from a preset in the library.

#. Create a deadline job object from the preset and submit the job using the deadline service.

Here is a complete code snippet for creating a deadline job with job and plugin info from the preset library:

.. code-block:: python
    :caption: hello_world.py

    import unreal
    from deadline_service import get_global_deadline_service_instance
    from deadline_job import DeadlineJob
    from deadline_utils import get_deadline_info_from_preset

    # preset name
    preset_name = "my_custom_preset_name"

    # loaded preset library
    preset_library = unreal.load_asset("/Game/path/to/DeadlineDataAsset.DeadlineDataAsset")

    # Get the job and plugin info from the data asset.
    # The returned data from this function is a tuple of dictionaries
    job_info, plugin_info = get_deadline_info_from_preset(preset_name, preset_library)

    # NOTE: Modify the job info or plugin info with sourced/dynamic data you wish to publish/submit to deadline.

    # Create a deadline job object. The Deadline service uses this object to track submitted job. This also
    # allows you to retrieve information about the submitted job without necessarily knowing the deadline job ID
    deadline_job = DeadlineJob(job_info, plugin-info)

    # Get the Deadline service
    # Submit the deadline job. This returns the deadline job id from the server. Be sure to handle any exceptions that
    # may occur during job submission
    try:
        deadline_service = get_global_deadline_service_instance()
        deadline_job_id = deadline_service.submit_job(deadline_job)
    except Exception:
        raise


.. note::
    Although we used a data asset in the code snippet, you can create a **DeadlineJob** from a custom job info and plugin info dictionary built on the fly.
    This is useful when you do not have any real use case for a data asset and your job info and plugin info is highly dynamic.
    The Deadline Service also has other helpful features like:

        * **Unreal Toolbar Menu Action**: This allows you to create a custom menu in the Deadline menu bar on the Unreal main menu.
        * **Deadline RPC Connection**: This allows you to create a bidirectional communication with a deadline process on a remote host.

Advanced
-----------

Create a Custom Toolbar Menu Action
+++++++++++++++++++++++++++++++++++

You can create a toolbar menu action on the Editor's main menu toolbar using the deadline service module with this Python snippet.
This will create a menu under Deadline. See :ref:`Toolbar menu API<toolbar_menu_api>`.

.. code-block:: python

    from deadline_menus import DeadlineToolBarMenu

    def _execute_action():
    """Callback method to execute an action"""

        print("Executing command")

    def register_menu_action():
    """This method creates and registers the menu actions"""

        toolbar = DeadlineToolBarMenu()

        # This is the label that is shown on the interface
        label = "Custom Menu"

        # This description is represented as a tooltip when hovering on the menu
        description = "My custom menu that performs an action"

        # Unique internal section name of the menu.
        internal_section_name = "CustomMenu"

        toolbar.register_submenu(
            internal_section_name,
            _execute_action,
            label_name=label,
            description=description
        )


.. note::
    It is recommended you create an *init_unreal.py* file in your plugin and use this file to auto register your menu action
    on startup.


|

Create a Deadline RPC connection
++++++++++++++++++++++++++++++++

This python snippet shows how to create an RPC connection to an external Deadline process. This
is typically useful when you want to process multiple jobs/actions within a single unreal session. Projects with long
editor startup times can benefit from this behavior. See :ref:`Base RPC API<rpc_api>`.

.. warning::
    Processing multiple jobs in a single Editor session can be memory intensive. Currently, the Editor does not have
    a good way to truly flush its memory without a full shutdown. Be sure to monitor this behavior when using this
    feature.

.. code-block:: python

    from deadline_rpc import BaseRPC

    class MyCustomRPC(BaseRPC):
        """
        Class to execute Deadline jobs using an RPC connection. The Base class instantiates a connection
        to the external process and makes sure the server is connected before proceeding.
        """

        # This port is an example port a process could be listening on and not representative
        # of the actual listening port of the deadline process. By default, if a port number is not
        # specified as an argument, the base class will look for `DEADLINE_RPC_PORT` in the process environment
        # variables.

        DEADLINE_PORT = 0000

        def __init__(self, port=DEADLINE_PORT, **kwargs):
            """Constructor"""
            super(MyCustomRPC, self).__init__(port=port, **kwargs)

        def execute(self):
            """
            Method to perform custom actions.
            This method is called when the base class is instantiated and is a required method.
            """

            # The base class has a `proxy` property that allows us to talk to the external process.
            # Print if we are currently connected to a server.
            print(self.proxy.is_connected())

            print("Performing my custom action")

            # Tell the Deadline process we are done with our task
            self.task_complete = True


.. warning::
    If you do not mark your current execution task as complete, the deadline process will assume you have
    a long running task and run indefinitely.

There are two ways to notify an external process a task is complete:

    * **task_complete** property on the class instance:
        When this property is set to *True*, this notifies the remote deadline process a task is complete.
        The Deadline process will mark the current task complete and execute the next task in the queue.

        .. code-block:: python

            self.task_complete = True


    * **__main__.__notify_task_complete__** global:
        This global behaves similarly to the *task_complete* property. However, it is useful when you have a fire-and-forget process
        that executes a callback when its complete. To use this global,
        *import __main__* and set the *__notify_task_complete__* property to True.

        .. code-block:: python

            import __main__

            if hasattr(__main__, "__notify_task_complete__"):
                __main__.__notify_task_complete__ = True

This framework and interface are designed to enable users to create complex Unreal jobs that execute remotely, with all the heavy lifting done by the framework itself.
They make no assumptions about the type of job executing. In theory, users can process *any* arbitrary Python-enabled job within the Unreal Editor session.

|

.. seealso::

     Unreal Deadine :ref:`Python API<python_api>`.
