#  Copyright Epic Games, Inc. All Rights Reserved

import os

from DeadlineUI.Controls.Scripting.DeadlineScriptDialog import (
    DeadlineScriptDialog,
)
from Deadline.Scripting import MonitorUtils


PLUGIN = "UnrealEngine5"
SCRIPT_DIALOG = None


def __main__(*args):
    """
    This creates a local copy of the serialized pipeline used to render an MRQ
    job. This is useful for debugging MRQ render jobs locally
    """
    global SCRIPT_DIALOG
    SCRIPT_DIALOG = DeadlineScriptDialog()
    SCRIPT_DIALOG.SetTitle("Unreal Engine Serialized Pipeline")
    SCRIPT_DIALOG.SetIcon(SCRIPT_DIALOG.GetIcon("Python"))

    selected_jobs = MonitorUtils.GetSelectedJobs()

    unsupported_jobs = []
    non_serialized_pipeline_jobs = []
    saved_manifest_files = []
    manifest_errors = []

    for job in selected_jobs:

        # Ignore any job that are not unreal Engine plugins
        if job.JobPlugin != PLUGIN:

            unsupported_jobs.append(job.JobName)

            continue

        # Make sure we have a serialized pipeline defined in the job plugin
        serialized_pipeline = job.GetJobExtraInfoKeyValue("serialized_pipeline")

        if not serialized_pipeline:
            non_serialized_pipeline_jobs.append(job.JobName)

            continue

        # Show a dialog where we want to save the Manifest file
        manifest_dir = SCRIPT_DIALOG.ShowFolderBrowser(os.path.expanduser("~"))

        # Do nothing if the dialog was closed/cancelled or returned None
        if not manifest_dir:
            continue
        filename = "{job}QueueManifest.utxt".format(job=job.JobName)
        filename = filename.replace(" ", "_")
        manifest_file = os.path.join(manifest_dir, filename)

        print("Saving manifest file to `{manifest}`".format(manifest=manifest_file))

        try:
            # Dump the manifest data into the manifest file
            with open(manifest_file, "w") as manifest:
                manifest.write(serialized_pipeline)

        except Exception as err:
            manifest_errors.append((job.JobName, str(err)))
            continue

        saved_manifest_files.append(manifest_file)

    if unsupported_jobs:
        # Show an unsupported message for the selected job
        SCRIPT_DIALOG.ShowMessageBox(
            "The selected jobs `{jobs}` were not executed with the expected "
            "plugin. Expected plugin is `{plugin}`".format(
                plugin=PLUGIN, jobs=", ".join(unsupported_jobs)
            ),
            "Unsupported Plugin",
        )

    if non_serialized_pipeline_jobs:
        # show a dialog for jobs that do not have a serialized pipeline in
        # the job info
        SCRIPT_DIALOG.ShowMessageBox(
            "The selected jobs `{jobs}` have no Movie Render Queue serialized "
            "pipeline defined.".format(jobs=", ".join(non_serialized_pipeline_jobs)),
            "Undefined Serialized MRQ Pipeline",
        )

    if manifest_errors:
        # notify about errors while creating the manifest file
        message = "An error occurred creating a manifest file for these jobs: "

        for job, err in manifest_errors:
            message += "\n{job}: {err}".format(job=job, err=err)

        SCRIPT_DIALOG.ShowMessageBox(
            message,
            "Manifest File Error",
        )

    if saved_manifest_files:
        # If all goes right, show a dialog about the location of the manifest
        # file
        SCRIPT_DIALOG.ShowMessageBox(
            "Manifest files saved successfully.\nPlace the manifest files in "
            "`<unreal_project>/Saved/MovieRenderPipeline` directory.\nUnreal "
            "will only load `.utxt` files from specific locations."
            "\nFiles: `{manifest}`".format(manifest=", ".join(saved_manifest_files)),
            "Save Manifest",
        )
