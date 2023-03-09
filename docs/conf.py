# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html
import os
import sys

# Code paths for documentation
code_paths = ['../deadline/plugins/UnrealEngine',
              '../unreal/EpicDeadlineService/Content/Python']

for path in code_paths:
    path = os.path.abspath(path)
    print(path)
    if os.path.exists(path):
        sys.path.insert(0, path)

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Unreal Engine Deadline'
copyright = '2023, Epic Games Inc'
author = 'Art Pipeline'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ['sphinx.ext.todo', 'sphinx.ext.viewcode', 'sphinx.ext.autodoc']

# Autodoc settings
autodoc_mock_imports = ['unreal', 'deadline_shared', 'Deadline', 'FranticX']

templates_path = ['_templates']
exclude_patterns = []


# -- Options for HTML html -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
