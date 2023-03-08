# Unreal Engine Deadline Plugin Documentation

This directory contains the source files used to build sphinx-docs for the Deadline plugin.

# Building the Documentation
To build the documentation and/or host the HTML in a separate location, follow these steps:

- Set your current working directory to the `docs` directory.
- Execute the following commands in your console:
  ```console
    pip install -r requirements.txt
    sphinx-build -b html ./ build
  ```

This will build an HTML-based sphinx doc into a folder called `build`. you 
can copy its context to you hosting location or select the `index.html` 
file to start browsing the documentation.
