# Copyright Epic Games, Inc. All Rights Reserved

# Built-in
import sys
from pathlib import Path

from deadline_utils import get_editor_deadline_globals
from deadline_service import DeadlineService

# Third-party
import unreal

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

# The asset registry may not be fully loaded by the time this is called, so we
# will wait until it has finished parsing all the assets in the project
# before we move on, otherwise attempts to look assets up may fail
# unexpectedly.
asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
if asset_registry.is_loading_assets():
    unreal.log_warning("Asset Registry is loading, waiting to complete...")
    asset_registry.wait_for_completion()

    unreal.log_warning("Asset Registry load complete!")

# Add the actions path to sys path
actions_path = Path(__file__).parent.joinpath("service_actions").as_posix()

if actions_path not in sys.path:
    sys.path.append(actions_path)

from service_actions import submit_job_action

# Register the menu from the render queue actions
submit_job_action.register_menu_action()
