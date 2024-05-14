Welcome to the Lengine

This is a small Game engine in development by Len Farag.
It will have an editor target that allows you to build the game in a developer window.
It will have an inspector, content browser, scene viewer, editor and client viewports, and a bunch of other things.

To use this project, you can create an application by placing a folder with your source files inside of the Applications folder.
Running GenerateProjectFiles.bat will then produce a project and solution file called Lengine.sln in the root directory you can run.
It uses Premake5 to generate the project, and that is supplied in the project, so no need to install Premake5 if you don't already
have it.

In your Application folder, you can create a Resources folder to store all of your resource files inside. This will automatically be linked
via a proprocessor define.

Using CleanProjectFiles.bat will clear the .vs files, the Generated/Project files and the Lengine.sln.
Everything inside of the Generated folder is completely ignored by the .gitignore, so you shouldn't have any issues submitting rubbish.