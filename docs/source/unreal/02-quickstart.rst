.. Copyright Epic Games, Inc. All Rights Reserved.

==========
Quickstart
==========

This quickstart guide demonstrates how to configure and submit a Deadline job.

Create Deadline Data Asset
--------------------------

This plugin creates a custom **Data Asset** type called **DeadlineJobPresetLibrary**, designed to be a collection of job submission data.
The preset library is a collection of Deadline job data in the form of key-value pairs. Each key represents the job preset name, and its corresponding value is the job data.
The job data consists of a Deadline **Job Info** and Deadline **Plugin Info**.

.. seealso::

   | `Job Info <https://docs.thinkboxsoftware.com/products/deadline/10.1/1_User%20Manual/manual/manual-submission.html#job-info-file-options>`_
   | `Plugin Info <https://docs.thinkboxsoftware.com/products/deadline/10.1/1_User%20Manual/manual/manual-submission.html#plug-in-info-file>`_

To create a data asset:

#. Right-click in the content browser and look for **Data Asset**. This asset type is usually located under **Miscellaneous**.
#. In the Classes search box, type **DeadlineJobPresetLibrary**.

    .. image:: _images/unreal_data_asset.png
        :align: center
        :width: 500px

    |

    .. image:: _images/unreal_deadline_preset.png
        :align: center
        :width: 500px

    |

#. Give your data asset a name representative of its use.
#. Open the data asset, add a new preset, and give it a descriptive name.

    .. image:: _images/unreal_deadline_sequence_preset.png
        :align: center
        :width: 1000px

    |

    .. _job-info-preset:

#. Expand the **Job Info Preset** section and fill in the necessary details for the Deadline job. See `Job Info`_ for the available options and their definition.

    .. note::
        To add additional job options in the interface, you can add the key-value pairs of the option to a custom mapping **Extra Job Options** in the **Job Info Preset**.

    .. _plugin-info-preset:

#. Expand the **Plugin Info Preset** section and input the parameters of your Deadline plugin as a key-value pair.
   See `Plugin Info`_ for examples of parameters for different Deadline plugins.
   It is important to know what parameters are required and optional for the specified Deadline plugin in the **Job Info Preset**.

#. Save the Data Asset.

|

Submit Deadline Job
------------------------

To submit a Deadline job:

#. In Unreal Editor's main menu bar, click the **Deadline** menu and select **Submit Deadline Job**.
#. Select your data asset from the **Preset Library** drop-down list.
#. Select the preset name from the list of options. The list is generated from the preset names in the preset library.
#. Click Submit.

    .. warning::
        If a job could not be submitted, make sure to check the output logs for details.

    .. image:: _images/unreal_deadline_data_asset.png
        :align: center
        :width: 1000px

