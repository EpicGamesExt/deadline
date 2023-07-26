# Copyright Epic Games, Inc. All Rights Reserved

# Built-in
import sys
from pathlib import Path

from deadline_utils import get_editor_deadline_globals
from deadline_service import DeadlineService

# Third-party
import unreal

deadline_service_tick_handle = None
asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
plugin_name = "DeadlineService"


def _load_plugin_after_asset_registry(delta_seconds):
    """
    This functions async waits for the asset registry to load before loading
    the plugin modules
    """
    global asset_registry
    if asset_registry.is_loading_assets():
        return

    global deadline_service_tick_handle

    unreal.unregister_slate_post_tick_callback(deadline_service_tick_handle)

    deadline_service_tick_handle = None

    global plugin_name

    unreal.log(f"Asset Registry is complete. Loading {plugin_name} plugin.")

    # Create a global instance of the deadline service. This is useful for
    # unreal classes that are not able to save the instance as an
    # attribute on the class. Because the Deadline Service is a singleton,
    # any new instance created from the service module will return the global
    # instance
    deadline_globals = get_editor_deadline_globals()

    try:
        deadline_globals["__deadline_service_instance__"] = DeadlineService()
    except Exception as err:
        raise RuntimeError(f"An error occurred creating a Deadline service instance. \n\tError: {str(err)}")

    from service_actions import submit_job_action

    # Register the menu from the render queue actions
    submit_job_action.register_menu_action()


# The asset registry may not be fully loaded by the time this is called,
# warn the user that attempts to look assets up may fail
# unexpectedly.
if asset_registry.is_loading_assets():
    unreal.log_warning(
        f"Asset Registry is still loading. The {plugin_name} plugin will "
        f"be loaded after the Asset Registry is complete."
    )

deadline_service_tick_handle = unreal.register_slate_post_tick_callback(_load_plugin_after_asset_registry)
