.. Copyright Epic Games, Inc. All Rights Reserved.

**************
Unreal Engine
**************

This Unreal Engine plugin enables you to submit Deadline jobs to a Deadline repository.
This documentation describes the plugin's functionality and how to use it to submit *any* arbitrary Deadline job.

.. warning::
    This plugin uses the Deadline Webservice for job submissions.

The Deadline plugin provides a custom Unreal **Data Asset** type that you can use to configure and store part or all of the data required for a Deadline job submission.
You can then use this asset to submit a job using a background process called the **Deadline Service**.

.. toctree::
   :caption: User Guide

   01-setup
   02-quickstart

.. toctree::
   :caption: Developer Guide

   03-developer_guide

.. toctree::
   :caption: API

   04-api
