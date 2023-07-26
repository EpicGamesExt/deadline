# Copyright Epic Games, Inc. All Rights Reserved

# Built-in
import sys
from pathlib import Path

# Third-party
import unreal

movie_pipeline_tick_handle = None
asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
plugin_name = "MoviePipelineDeadline"


def _load_plugin_after_asset_registry(delta_seconds):
    """
    This functions async waits for the asset registry to load before loading
    the plugin modules
    """
    global asset_registry
    if asset_registry.is_loading_assets():
        return

    global movie_pipeline_tick_handle

    unreal.unregister_slate_post_tick_callback(movie_pipeline_tick_handle)

    movie_pipeline_tick_handle = None

    global plugin_name

    unreal.log(f"Asset Registry is complete. Loading {plugin_name} plugin.")

    import remote_executor
    import mrq_cli

    from pipeline_actions import render_queue_action

    # Register the menu from the render queue actions
    render_queue_action.register_menu_action()


# The asset registry may not be fully loaded by the time this is called,
# warn the user that attempts to look assets up may fail
# unexpectedly.
if asset_registry.is_loading_assets():
    unreal.log_warning(
        f"Asset Registry is still loading. The {plugin_name} plugin will "
        f"be loaded after the Asset Registry is complete."
    )

movie_pipeline_tick_handle = unreal.register_slate_post_tick_callback(_load_plugin_after_asset_registry)
