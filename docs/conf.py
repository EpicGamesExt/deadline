# Importing necessary modules
import os
import sys

# Paths to the code for which the documentation is to be generated
code_paths = ['../deadline/plugins/UnrealEngine',
              '../unreal/UnrealDeadlineService/Content/Python',
              '../unreal/MoviePipelineDeadline/Content/Python']

# Adding the code paths to the system path
for path in code_paths:
    path = os.path.abspath(path)
    print(path)
    if os.path.exists(path):
        sys.path.insert(0, path)

# Project information
project = 'Unreal Engine Deadline'  # The name of the project
copyright = '2023, Epic Games Inc'  # Copyright information
author = 'Art Pipeline'  # The author of the project

# Extensions for Sphinx to use when building the documentation
extensions = ['sphinx.ext.todo', 'sphinx.ext.viewcode', 'sphinx.ext.autodoc']

# Mock imports for autodoc to prevent import errors
autodoc_mock_imports = ['unreal', 'deadline_shared', 'Deadline', 'FranticX']

# Path to the templates
templates_path = ['_templates']
# Patterns to exclude from the documentation
exclude_patterns = []

# Theme for the HTML documentation
html_theme = 'sphinx_rtd_theme'
# Path to static files for the HTML documentation
html_static_path = ['_static']
