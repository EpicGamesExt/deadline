.. Copyright Epic Games, Inc. All Rights Reserved.

**********
Deadline
**********

The Unreal Engine plugin enables users to execute *any* arbitrary Unreal job.
In this documentation, we will discuss the design philosophy of the plugin, as well as how to use it and its expectations.

Unreal Engine is a multi-purpose digital content creation tool designed to serve many different clients, such as game developers, filmmakers, and product designers.
With the introduction of Python in the Engine, its adoption has increased by many institutions due to the increased scriptability of the Engine.
This has allowed many companies and users to write Python code to execute render jobs or even integrate the Engine into their pipelines.
To support this scripting on a remote process like Deadline, the plugin needed to be built on several foundations.

.. _design:

==================
Design Foundations
==================

* The Deadline plugin cannot assume the type of Unreal Engine job to execute.

* All executed commands need to be reproducible on a local machine.

* The plugin cannot make implicit assumptions about the location of the Engine binaries,
  but instead, provide tools to resolve implicit locations using built-in Deadline APIs.

* The plugin needs to support cases where multiple tasks can be executed in a single Unreal Process or Session.
  This is useful for cases where the startup times for a project are very long, and you don't want to pay the loading cost for every task.

* The plugin should not have any assumptions about Unreal Engine built into it.

* The plugin should not make any assumptions about the current platform. It should use Deadline's API as much as possible for multi-platform interfaces.

Based on these foundations, the plugin is a general-purpose plugin enabling many different use cases for remote Unreal Engine job types.

.. toctree::
   :caption: User guide

   01-user_guide

.. toctree::
   :caption: Developer guide

   02-developer_guide

.. toctree::
   :caption: API

   03-api
