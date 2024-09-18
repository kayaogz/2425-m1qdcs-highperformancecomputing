# [M1 QDCS] High performance computing (2.5 ECTS)

This is an introductory course on the basics of high performance and parallel computing, with a particular focus on multicore and vector parallelism on a single machine. Apart from parallelism, many multicore architecture details and various code optimization techniques will be covered throughout the course.

# Course organization

We will have seven sessions of 3 hours each (plus mini-exam, c.f. below). The first session is an introduction to high performance computing. The rest will be a mix of course (around 1h-1h30) and lab session (around 1h30-2h) on that subject. You might not be able to finish all exercises during the lab session (which is completely normal); you should make sure that you can understand and finish all basic exercises (first two or three) during the session so that you feel more confident to finish the rest on your own.

## Session 1: Introduction to high performance computing
## Session 2: Introduction to multithreaded parallelism using OpenMP
## Session 3: Advanced OpenMP
## Session 4: Introduction to vector programming using AVX2
## Session 5: Advanced vectorization and code optimization techniques I
## Session 6: Advanced vectorization and code optimization techniques II
## Session 7: Parallel linear algebra libraries for scientific computing

# Evaluation

## Graded lab assignments
Each lab assignment should be sent to the address `oguz.kaya[at]universite-paris-saclay.fr` with the subject format **"M1QDCSHPC LABX SURNAME(s) Name(s)"** (e.g., **M1QDCSHPC LAB3 ARNAULT-BARRAT Jean-François**) by 23:59 Sunday following the lab session. Solutions will be posted on the git repository just after this deadline; therefore, do not submit late! Please follow this format for the email subject **to the letter** as I will employ filters to sort these submissions.

Please only attach source files (*.cpp) to your email, **one file per exercise** (e.g., if the assignment has four exercises, your submission must have four .cpp files), and please **do not zip the files**!

If there are plots in some exercises, **do not** include them in the submission; those are only for your better understanding of the code's behavior, not for evaluation. If there are other questions/interpretations requiring textual response as part of an exercise, you can put your  at the very beginning of the corresponding source file (*.cpp) in a comment section.

Out of all lab assignments, two of them will be randomly selected for grading, each providing up to 2/20 points.

## End-of-session mini-exams
At the end of weeks 4, 5, 6, 7, there will be mini-exams for 4/20 points each. Three types of exercises that might appear on these mini-exams are:

**Debugging:** Given a complete code with multiple bugs, the goal is to identify each bug (without correcting them), say what the problem is in a single sentence, and point out the corresponding line(s) in the code.

**Output:** Given a complete code, the goal is to find out what the program could potentially display. Indeed, this requires a thorough understanding of how the code works. There might be multiple possibilities, in which case you should indicate all of them.

**Coding:** A squeleton code or a function signature will be given for a specific task, and you will be expected to provide a parallel/optimized code that performs the intended task.

You can bring four A4 sheets to the mini-exam (both sides); no other material is allowed.

# Technical tips

Text editor: If you do not have a "preferred" text editor for development already, I recommend Visual Studio Code: https://code.visualstudio.com . You can follow the official tutorial for the basics: https://www.youtube.com/watch?v=B-s71n0dHUk&list=PLj6YeMhvp2S5UgiQnBfvD7XgOMKs3O_G6

You will need a GCC compiler for this course, aan x86-based machine for the vectorization part of the course. If you are using Linux on an Intel/AMD machine, you have all you need. 

On Windows, you will need to install Ubuntu using WSL: https://www.youtube.com/watch?v=wjbbl0TTMeo

On MacOS, you will need to first install Homebrew: https://www.youtube.com/watch?v=IWJKRmFLn-g

Then install GCC using Homebrew with the command: **brew install gcc**. Note that to compile using GCC on MacOS, you should use the command **g++-14** instead of **g++** (which defaults to the clang compiler that does not come with OpenMP).
