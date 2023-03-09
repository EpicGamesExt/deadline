# Copyright Epic Games, Inc. All Rights Reserved

# Built-in
import argparse
from getpass import getuser

# Internal
from deadline_service import get_global_deadline_service_instance
from deadline_job import DeadlineJob
from deadline_menus import DeadlineToolBarMenu
from deadline_utils import get_deadline_info_from_preset

# Third Party
import unreal

# Editor Utility Widget path
# NOTE: This is very fragile and can break if naming or pathing changes
EDITOR_UTILITY_WIDGET = "/EpicDeadlineService/Widgets/DeadlineJobSubmitter"


def _launch_job_submitter():
    """
    Callback to execute to launch the job submitter
    """
    unreal.log("Launching job submitter.")

    submitter_widget = unreal.EditorAssetLibrary.load_asset(EDITOR_UTILITY_WIDGET)

    # Get editor subsystem
    subsystem = unreal.get_editor_subsystem(unreal.EditorUtilitySubsystem)

    # Spawn the submitter widget
    subsystem.spawn_and_register_tab(submitter_widget)


def register_menu_action():
    """
    Creates the toolbar menu
    """

    if not _validate_euw_asset_exists():
        unreal.log_error(
            f"EUW {EDITOR_UTILITY_WIDGET} does not exist in the Asset registry!"
        )
        return

    toolbar = DeadlineToolBarMenu()

    toolbar.register_submenu(
        "SubmitDeadlineJob",
        _launch_job_submitter,
        label_name="Submit Deadline Job",
        description="Submits a job to Deadline"
    )


def _validate_euw_asset_exists():
    """
    Make sure our reference editor utility widget exists in
    the asset registry
    :returns: Array(AssetData) or None
    """

    asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
    asset_data = asset_registry.get_assets_by_package_name(
        EDITOR_UTILITY_WIDGET,
        include_only_on_disk_assets=True
    )

    if asset_data is not None:
        return True

    return False


def _execute_submission(args):
    """
    Creates and submits a job to Deadline
    :param args: Commandline args
    """

    unreal.log("Executing job submission")

    job_info, plugin_info = get_deadline_info_from_preset(
        args.preset_name,
        unreal.load_asset(args.preset_library)
    )

    # Set the Author of the job
    if not job_info.get("UserName", None):
        job_info["UserName"] = getuser()

    # Create a Deadline job from the selected preset library
    deadline_job = DeadlineJob(job_info, plugin_info)

    deadline_service = get_global_deadline_service_instance()

    # Submit the Deadline Job
    job_id = deadline_service.submit_job(deadline_job)

    unreal.log(f"Deadline job submitted. JobId: {job_id}")


if __name__ == "__main__":
    unreal.log("Executing job submitter action")

    parser = argparse.ArgumentParser(
        description="Submits a job to Deadline",
        add_help=False,
    )

    parser.add_argument(
        "--preset_name",
        type=str,
        help="Deadline Preset Name"
    )

    parser.add_argument(
        "--preset_library",
        type=str,
        help="Deadline Preset Library"
    )

    parser.set_defaults(func=_execute_submission)

    # Parse the arguments and execute the function callback
    arguments = parser.parse_args()
    arguments.func(arguments)
