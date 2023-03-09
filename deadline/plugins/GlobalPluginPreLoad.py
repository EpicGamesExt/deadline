#!/usr/bin/env python3
#  Copyright Epic Games, Inc. All Rights Reserved

import os
import sys

from Deadline.Scripting import RepositoryUtils


# This is executed on the worker prior to it attempting to execute a task.
def __main__(*_):
    # Add the plugin directory to the python path
    libs_path = os.path.normpath(
        os.path.join(
            RepositoryUtils.GetCustomPluginsDirectory(),
            "libs"
        )
    )

    if libs_path not in sys.path:
        print(
            "Adding custom libs path `{path}` to Python env".format(
                path=libs_path
            )
        )
        sys.path.append(libs_path)
        print("sys.path: {}".format(sys.path))
