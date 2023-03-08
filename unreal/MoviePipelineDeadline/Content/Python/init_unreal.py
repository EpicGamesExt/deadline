# Copyright Epic Games, Inc. All Rights Reserved

# Built-in
import sys
from pathlib import Path

# Third-party
import unreal

################################################################################
# Always execute this first to make sure the Asset registry has picked up all
# assets before continuing


# The asset registry may not be fully loaded by the time this is called, so we
# will wait until it has finished parsing all the assets in the project
# before we move on, otherwise attempts to look assets up may fail
# unexpectedly.
asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
if asset_registry.is_loading_assets():
    unreal.log_warning("Asset Registry is loading, waiting to complete...")
    asset_registry.wait_for_completion()

    unreal.log_warning("Asset Registry load complete!")

###############################################################################

import remote_executor
import mrq_cli

# Add the actions path to sys path
actions_path = Path(__file__).parent.joinpath("pipeline_actions").as_posix().lower()

if actions_path not in sys.path:
    sys.path.append(actions_path)

from pipeline_actions import render_queue_action

# Register the menu from the render queue actions
render_queue_action.register_menu_action()
