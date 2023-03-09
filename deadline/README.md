# Unreal Engine Deadline Repository Plugin

This Unreal Engine plugin enables users to execute any scriptable task in Unreal Engine on Deadline

> **_Note:_** It was tested with Deadline version 10.1.23.6, 
> and does not currently support processes such as perforce syncs. 
> However, this functionality can be easily added through a `JobPreLoad.py` interface.


# Usage

To use the Deadline plugin:

- Copy the contents of the `plugins` and `scripts` directory into the `customs` folder
 in your Deadline repository.

- Use the `Unreal Engine` submitter in the Deadline `Submit` menu to submit a 
  commandline job to Deadline.

# Documentation

Extensive documentation on the plugin and its functionality can be found in the `docs` directory in the repository.
