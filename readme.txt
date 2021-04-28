// CSCI 2270 - Final Project: miniGit
// Authors: Stefano Matarangas, Md Mahmud, Lev Polonsky

=========================================== PROJECT DESCRIPTION ===========================================

    This project is a version control system that mirrors Git and allows users to add, remove and commit changes
to files that the user is working on. The functions are add, remove, commit and checkout. The functionality 
of these functions are explained in the Functionality section. When first run, the program asks if the user 
wants to initialize the repo for the miniGit. If they agree to initialize, then they will be prompted with 
a menu with 5 options: add, remove, commit, checkout, quit. The user inputs in their selection and they 
will be prompted with further requirements based on their choice.


============================================== FUNCTIONALITY ==============================================

    * INIT: Initializes the miniGit repository.

    * ADD: Adds file to be tracked by miniGit.

    * REMOVE: Remove files that are currently tracked by miniGit.

    * COMMIT: Commits all changes in the added files to the repository, backing up a 
              current version of them that could be accessed after other changes are made.
    
    * CHECKOUT: Checks out a different commit of the project.
