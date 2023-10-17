#!/usr/bin/env python3
#  Copyright Epic Games, Inc. All Rights Reserved
import os
import sys
import json
from System.IO import Path

from Deadline.Scripting import RepositoryUtils, ClientUtils, FrameUtils
from DeadlineUI.Controls.Scripting.DeadlineScriptDialog import DeadlineScriptDialog


########################################################################
## Globals
########################################################################
SCRIPT_DIALOG = None
SETTINGS = None

# Add the plugin directory to the python path
path = os.path.join(RepositoryUtils.GetPluginsDirectory(), "UnrealEngine5", "ue_utils")

if path not in sys.path:
    print("Adding `{path}` to python path".format(path=path))
    sys.path.insert(0, path)

# Deferred import to prevent script from breaking every job
import submit_deadline_job

def __main__(*args):
    global SCRIPT_DIALOG
    global SETTINGS

    SCRIPT_DIALOG = DeadlineScriptDialog()
    SCRIPT_DIALOG.SetTitle("Submit Unreal Engine Commandline Job to Deadline")
    SCRIPT_DIALOG.SetIcon(SCRIPT_DIALOG.GetIcon("Python"))

    SCRIPT_DIALOG.AddGrid()
    SCRIPT_DIALOG.AddControlToGrid(
        "Separator1", "SeparatorControl", "Job Description", 0, 0, colSpan=2
    )

    SCRIPT_DIALOG.AddControlToGrid(
        "NameLabel",
        "LabelControl",
        "Job Name",
        1,
        0,
        "The name of your job. This is optional, and if left blank, it will default to 'Untitled'.",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid("NameBox", "TextControl", "Untitled", 1, 1)

    SCRIPT_DIALOG.AddControlToGrid(
        "CommentLabel",
        "LabelControl",
        "Comment",
        2,
        0,
        "A simple description of your job. This is optional and can be left blank.",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid("CommentBox", "TextControl", "", 2, 1)

    SCRIPT_DIALOG.AddControlToGrid(
        "DepartmentLabel",
        "LabelControl",
        "Department",
        3,
        0,
        "The department you belong to. This is optional and can be left blank.",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid("DepartmentBox", "TextControl", "", 3, 1)
    SCRIPT_DIALOG.EndGrid()

    SCRIPT_DIALOG.AddGrid()
    SCRIPT_DIALOG.AddControlToGrid(
        "Separator2", "SeparatorControl", "Job Options", 0, 0, colSpan=3
    )

    SCRIPT_DIALOG.AddControlToGrid(
        "PoolLabel",
        "LabelControl",
        "Pool",
        1,
        0,
        "The pool that your job will be submitted to.",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid("PoolBox", "PoolComboControl", "none", 1, 1)

    SCRIPT_DIALOG.AddControlToGrid(
        "SecondaryPoolLabel",
        "LabelControl",
        "Secondary Pool",
        2,
        0,
        "The secondary pool lets you specify a Pool to use if the primary Pool does not have any available Slaves.",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid(
        "SecondaryPoolBox", "SecondaryPoolComboControl", "", 2, 1
    )

    SCRIPT_DIALOG.AddControlToGrid(
        "GroupLabel",
        "LabelControl",
        "Group",
        3,
        0,
        "The group that your job will be submitted to.",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid(
        "GroupBox", "GroupComboControl", "none", 3, 1
    )

    SCRIPT_DIALOG.AddControlToGrid(
        "PriorityLabel",
        "LabelControl",
        "Priority",
        4,
        0,
        "A job can have a numeric priority ranging from 0 to 100, where 0 is the lowest priority and 100 is the highest priority.",
        False,
    )
    SCRIPT_DIALOG.AddRangeControlToGrid(
        "PriorityBox",
        "RangeControl",
        RepositoryUtils.GetMaximumPriority() / 2,
        0,
        RepositoryUtils.GetMaximumPriority(),
        0,
        1,
        4,
        1,
    )

    SCRIPT_DIALOG.AddControlToGrid(
        "TaskTimeoutLabel",
        "LabelControl",
        "Task Timeout",
        5,
        0,
        "The number of minutes a slave has to render a task for this job before it requeues it. Specify 0 for no limit.",
        False,
    )
    SCRIPT_DIALOG.AddRangeControlToGrid(
        "TaskTimeoutBox", "RangeControl", 0, 0, 1000000, 0, 1, 5, 1
    )
    SCRIPT_DIALOG.AddSelectionControlToGrid(
        "AutoTimeoutBox",
        "CheckBoxControl",
        False,
        "Enable Auto Task Timeout",
        5,
        2,
        "If the Auto Task Timeout is properly configured in the Repository Options, then enabling this will allow a task timeout to be automatically calculated based on the render times of previous frames for the job. ",
    )

    SCRIPT_DIALOG.AddControlToGrid(
        "MachineLimitLabel", "LabelControl", "Machine Limit", 6, 0, "", False
    )
    SCRIPT_DIALOG.AddRangeControlToGrid(
        "MachineLimitBox", "RangeControl", 0, 0, 1000000, 0, 1, 6, 1
    )
    SCRIPT_DIALOG.AddSelectionControlToGrid(
        "IsBlacklistBox",
        "CheckBoxControl",
        False,
        "Machine List Is A Blacklist",
        6,
        2,
        "",
    )

    SCRIPT_DIALOG.AddControlToGrid(
        "MachineListLabel",
        "LabelControl",
        "Machine List",
        7,
        0,
        "Use the Machine Limit to specify the maximum number of machines that can render your job at one time. Specify 0 for no limit.",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid(
        "MachineListBox", "MachineListControl", "", 7, 1, colSpan=2
    )

    SCRIPT_DIALOG.AddControlToGrid(
        "LimitGroupLabel",
        "LabelControl",
        "Limits",
        8,
        0,
        "The Limits that your job requires.",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid(
        "LimitGroupBox", "LimitGroupControl", "", 8, 1, colSpan=2
    )

    SCRIPT_DIALOG.AddControlToGrid(
        "DependencyLabel",
        "LabelControl",
        "Dependencies",
        9,
        0,
        "Specify existing jobs that this job will be dependent on. This job will not start until the specified dependencies finish rendering. ",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid(
        "DependencyBox", "DependencyControl", "", 9, 1, colSpan=2
    )

    SCRIPT_DIALOG.AddControlToGrid( "FramesLabel", "LabelControl", "Frame List", 10, 0, "The list of frames to render.", False )
    SCRIPT_DIALOG.AddControlToGrid( "FramesBox", "TextControl", "", 10, 1 )
    SCRIPT_DIALOG.SetValue( "FramesBox", 0 )

    SCRIPT_DIALOG.AddControlToGrid( "ChunkSizeLabel", "LabelControl", "Frames Per Task", 11, 0, "This is the number of frames that will be rendered at a time for each job task.", False )
    SCRIPT_DIALOG.AddRangeControlToGrid( "ChunkSizeBox", "RangeControl", 1, 1, 1000000, 0, 1, 11, 1 )

    SCRIPT_DIALOG.AddControlToGrid(
        "OnJobCompleteLabel",
        "LabelControl",
        "On Job Complete",
        12,
        0,
        "If desired, you can automatically archive or delete the job when it completes. ",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid(
        "OnJobCompleteBox", "OnJobCompleteControl", "Nothing", 12, 1
    )
    SCRIPT_DIALOG.AddSelectionControlToGrid(
        "SubmitSuspendedBox",
        "CheckBoxControl",
        False,
        "Submit Job As Suspended",
        12,
        2,
        "If enabled, the job will submit in the suspended state. This is useful if you don't want the job to start rendering right away. Just resume it from the Monitor when you want it to render. ",
    )
    SCRIPT_DIALOG.EndGrid()

    SCRIPT_DIALOG.AddTabControl("Tabs", 0, 0)
    SCRIPT_DIALOG.AddTabPage("Unreal")

    SCRIPT_DIALOG.AddGrid()
    SCRIPT_DIALOG.AddControlToGrid(
        "Separator3", "SeparatorControl", "Editor", 0, 0, colSpan=2
    )
    SCRIPT_DIALOG.AddControlToGrid(
        "ExecutableLabel",
        "LabelControl",
        "Executable",
        1,
        0,
        "e.g c:/Root/Engine/Binaries/Win64/UnrealEditor-Cmd.exe",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid("ExecutableBox", "TextControl", "", 1, 1)
    SCRIPT_DIALOG.AddControlToGrid(
        "UProjectLabel",
        "LabelControl",
        "Project File",
        2,
        0,
        "Project File e.g c:/Root/MyUproject.uproject",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid("UProjectBox", "TextControl", "", 2, 1)
    SCRIPT_DIALOG.AddControlToGrid(
        "ArgsLabel",
        "LabelControl",
        "CommandLine Arguments",
        3,
        0,
        "Commandline arguments for unreal",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid("ArgsBox", "TextControl", "", 3, 1)

    SCRIPT_DIALOG.EndGrid()

    SCRIPT_DIALOG.EndTabPage()

    SCRIPT_DIALOG.AddTabPage("Perforce")

    SCRIPT_DIALOG.AddGrid()
    SCRIPT_DIALOG.AddControlToGrid(
        "Separator3", "SeparatorControl", "Workspace", 0, 0, colSpan=3
    )
    perforce_box = SCRIPT_DIALOG.AddSelectionControlToGrid(
        "PerforceBox",
        "CheckBoxControl",
        False,
        "Perforce",
        1,
        0,
        "Sync Perforce",
    )
    # Comment out to enable perforce
    SCRIPT_DIALOG.SetEnabled("PerforceBox", False)

    perforce_box.ValueModified.connect(_enable_perforce)

    SCRIPT_DIALOG.AddControlToGrid(
        "StreamLabel",
        "LabelControl",
        "Project Stream",
        2,
        0,
        "Perforce stream for the workspace",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid("StreamBox", "TextControl", "", 2, 1)

    SCRIPT_DIALOG.AddControlToGrid(
        "ProjectLabel",
        "LabelControl",
        "Project File",
        3,
        0,
        "Unreal project file name e.g MyProject.uproject",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid("ProjectBox", "TextControl", "", 3, 1)
    SCRIPT_DIALOG.AddControlToGrid(
        "ChangelistLabel",
        "LabelControl",
        "Changelist",
        4,
        0,
        "Changelist to Sync Workspace",
        False,
    )
    SCRIPT_DIALOG.AddControlToGrid("ChangelistBox", "TextControl", "", 4, 1)
    sync_latest_box = SCRIPT_DIALOG.AddSelectionControlToGrid(
        "SyncLatestBox",
        "CheckBoxControl",
        False,
        "Latest",
        4,
        2,
        "Sync Workspace to Latest Changelist",
    )
    sync_latest_box.ValueModified.connect(_sync_latest)
    SCRIPT_DIALOG.AddSelectionControlToGrid(
        "ForceFullSyncBox",
        "CheckBoxControl",
        False,
        "Force Full Sync",
        5,
        0,
        "Force Sync Workspace",
    )
    SCRIPT_DIALOG.AddSelectionControlToGrid(
        "BuildLocalBox",
        "CheckBoxControl",
        True,
        "Build Local",
        6,
        0,
        "Build Local",
    )
    SCRIPT_DIALOG.EndGrid()
    SCRIPT_DIALOG.EndTabPage()
    SCRIPT_DIALOG.EndTabControl()

    SCRIPT_DIALOG.AddGrid()
    SCRIPT_DIALOG.AddHorizontalSpacerToGrid("HSpacer1", 0, 0)
    submit_button = SCRIPT_DIALOG.AddControlToGrid(
        "SubmitButton", "ButtonControl", "Submit", 0, 1, expand=False
    )
    submit_button.ValueModified.connect(submit_button_pressed)
    close_button = SCRIPT_DIALOG.AddControlToGrid(
        "CloseButton", "ButtonControl", "Close", 0, 2, expand=False
    )
    close_button.ValueModified.connect(SCRIPT_DIALOG.closeEvent)
    SCRIPT_DIALOG.EndGrid()

    SETTINGS = (
        "DepartmentBox",
        "CategoryBox",
        "PoolBox",
        "SecondaryPoolBox",
        "GroupBox",
        "PriorityBox",
        "MachineLimitBox",
        "IsBlacklistBox",
        "MachineListBox",
        "LimitGroupBox",
        "StreamBox",
        "ProjectBox",
        "ArgsBox",
        "ExecutableBox",
        "UProjectBox",
        "ChangelistBox",
        "SyncLatestBox",
        "ForceFullSyncBox",
        "BuildLocalBox"
    )
    SCRIPT_DIALOG.LoadSettings(GetSettingsFilename(), SETTINGS)
    SCRIPT_DIALOG.EnabledStickySaving(SETTINGS, GetSettingsFilename())

    # Make sure the dialogs are in the correct state
    _sync_latest()
    _enable_perforce()

    SCRIPT_DIALOG.ShowDialog(False)


def GetSettingsFilename():
    return Path.Combine(
        ClientUtils.GetUsersSettingsDirectory(), "UnrealEngineSettings.ini"
    )


def _sync_latest(*args):
    """Enable or disable Changelist box"""
    global SCRIPT_DIALOG

    sync_latest = bool(SCRIPT_DIALOG.GetValue("SyncLatestBox"))

    # Reverse the check state
    SCRIPT_DIALOG.SetEnabled("ChangelistBox", bool(not sync_latest))


def _enable_perforce(*args):
    """Enable Perforce Sync"""
    global SCRIPT_DIALOG

    perforce = bool(SCRIPT_DIALOG.GetValue("PerforceBox")) 

    SCRIPT_DIALOG.SetEnabled("StreamBox", perforce)
    SCRIPT_DIALOG.SetEnabled("ProjectBox", perforce)
    SCRIPT_DIALOG.SetEnabled("ChangelistBox", perforce)
    SCRIPT_DIALOG.SetEnabled("SyncLatestBox", perforce)
    SCRIPT_DIALOG.SetEnabled("ForceFullSyncBox", perforce)
    SCRIPT_DIALOG.SetEnabled("BuildLocalBox", perforce)


def submit_button_pressed(*args):
    frames = SCRIPT_DIALOG.GetValue("FramesBox")
    if not FrameUtils.FrameRangeValid(frames):
        SCRIPT_DIALOG.ShowMessageBox(f"Frame range `{frames}` is not valid" "Error")
        return

    # Create job dictionary
    job_info = {
        "Plugin": "UnrealEngine5",
        "Name": SCRIPT_DIALOG.GetValue("NameBox"),
        "Comment": SCRIPT_DIALOG.GetValue("CommentBox"),
        "Department": SCRIPT_DIALOG.GetValue("DepartmentBox"),
        "Pool": SCRIPT_DIALOG.GetValue("PoolBox"),
        "SecondaryPool": SCRIPT_DIALOG.GetValue("SecondaryPoolBox"),
        "Group": SCRIPT_DIALOG.GetValue("GroupBox"),
        "Priority": SCRIPT_DIALOG.GetValue("PriorityBox"),
        "TaskTimeoutMinutes": SCRIPT_DIALOG.GetValue("TaskTimeoutBox"),
        "EnableAutoTimeout": SCRIPT_DIALOG.GetValue("AutoTimeoutBox"),
        "MachineLimit": SCRIPT_DIALOG.GetValue("MachineLimitBox"),
        "LimitGroups": SCRIPT_DIALOG.GetValue("LimitGroupBox"),
        "JobDependencies": SCRIPT_DIALOG.GetValue("DependencyBox"),
        "OnJobComplete": SCRIPT_DIALOG.GetValue("OnJobCompleteBox"),
        "Frames": frames,
        "ChunkSize": SCRIPT_DIALOG.GetValue("ChunkSizeBox"),
    }

    if bool(SCRIPT_DIALOG.GetValue("IsBlacklistBox")):
        job_info.update({"Blacklist": SCRIPT_DIALOG.GetValue("MachineListBox")})
    else:
        job_info.update({"Whitelist": SCRIPT_DIALOG.GetValue("MachineListBox")})

    if bool(SCRIPT_DIALOG.GetValue("SubmitSuspendedBox")):
        job_info.update({"InitialStatus": "Suspended"})

    if bool(SCRIPT_DIALOG.GetValue("PerforceBox")):

        # Create Plugin dictionary
        workspace = {
            "PerforceStream": SCRIPT_DIALOG.GetValue("StreamBox"),
            "ProjectFile": SCRIPT_DIALOG.GetValue("ProjectBox"),
            "ForceSync": str(SCRIPT_DIALOG.GetValue("ForceFullSyncBox")),
            "BuildLocal": str(SCRIPT_DIALOG.GetValue("BuildLocalBox")),
            "Changelist": "latest",
        }
        if SCRIPT_DIALOG.GetValue("ChangelistBox"):
            workspace.update(
                {"Changelist": SCRIPT_DIALOG.GetValue("ChangelistBox")}
            )

        job_info.update(
            {
                "ExtraInfoKeyValue0": "perforce={workspace}".format(
                    workspace=json.dumps(workspace)
                )
            }
        )

    # Create Plugin dictionary
    plugin_info = {
        "Executable": SCRIPT_DIALOG.GetValue("ExecutableBox"),
        "ProjectFile": SCRIPT_DIALOG.GetValue("UProjectBox"),
        "CommandLineArguments": SCRIPT_DIALOG.GetValue("ArgsBox"),
        "CommandLineMode": "true"
    }
    print("Submitting job")
    results = submit_deadline_job.submit_job("UnrealEngine5", job_info,plugin_info)

    SCRIPT_DIALOG.ShowMessageBox(results, "Submission Results")
