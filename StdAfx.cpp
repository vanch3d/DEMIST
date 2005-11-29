// stdafx.cpp : source file that includes just the standard includes
//	Simul.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"



/*!
\mainpage 

 The simulation model (by CSimModel, see figure \latexonly figure~\ref{fig:Organigram} \endlatexonly).
\latexonly
\begin{figure}[p]% will be the left-side figure
    \begin{leftfullpage}
    \centering{
    \vfill%
    \includegraphics[angle=90,scale=.70]{fig/Archi_dv}
    \vfill}
    \caption{An overview of classes used in \DEMIST}\label{fig:Archi_Dev}
    \end{leftfullpage}

\end{figure}

\begin{figure}[p]% will be the right-side figure
    \begin{fullpage}
    \centering{
    \vfill%
    \includegraphics[angle=90,scale=.68]{fig/organig}
    \vfill}
    \caption{An overview of classes used in \DEMIST}\label{fig:Organigram}
    \end{fullpage}
\end{figure}

\begin{longtable}[c]{|p{5cm}|p{5cm}|}\caption{Matching between \DEMIST models and OOP Classes}%
    \hline
    {\footnotesize{\textbf{Model / Part of Model}}} &
    {\footnotesize{\textbf{Class}}} \\
    \hline\noalign{\smallskip}
    \multicolumn{2}{c}{\textbf{Simulation model}}\\
    \noalign{\smallskip}\hline
    \footnotesize{Simulation Model} &  \footnotesize{\textCmd{CSimModel}}\\
    \footnotesize{Formal Representation} &  \footnotesize{\textCmd{CMdEquation}}\\
    \footnotesize{Equation} &  -\\
    \footnotesize{Entities} &  \footnotesize{\textCmd{CModelObj}}\\
    \footnotesize{Variables} &  \footnotesize{\textCmd{CModelVar}}\\
   \footnotesize{Parameters} &  \footnotesize{\textCmd{CModelPar}}\\
    \hline\noalign{\smallskip}
   \multicolumn{2}{c}{\textbf{Context model}}\\
    \noalign{\smallskip}\hline
    \footnotesize{Context Model} &  \footnotesize{\textCmd{CInstModel}}\\
    \footnotesize{\LU} &  \footnotesize{\textCmd{CLearningUnit}}\\
    \footnotesize{Experimental Set} &  \footnotesize{\textCmd{CMdExpertSet}}\\
    \footnotesize{External Representation} &  \footnotesize{\textCmd{CExternRepUnit}}\\
    \hline\noalign{\smallskip}
    \multicolumn{2}{c}{\textbf{Instructional model}}\\
    \noalign{\smallskip}\hline
    \footnotesize{Instructional model} &  -\\
    \footnotesize{\Aggs} &  \footnotesize{\textCmd{CMdData} (\textCmd{CMdVarData}, \textCmd{CMdParData})}\\
    \footnotesize{Activities} &  \footnotesize{\textCmd{CTimerBreakPoint}}\\
    \footnotesize{User's data} &  \footnotesize{\textCmd{CUserData}}\\
    \hline\noalign{\smallskip}
    \multicolumn{2}{c}{\textbf{Interaction model}}\\
    \noalign{\smallskip}\hline
    \footnotesize{Interaction model} & - \\
    \footnotesize{ER Interface} &  \footnotesize{\textCmd{CView *}}\\
    \footnotesize{Outcomes} &  \footnotesize{\textCmd{COutputs }}\\
    \footnotesize{Actions} &  \footnotesize{\textCmd{CTranslation}}\\
    \footnotesize{Formats} &  \footnotesize{\textCmd{CFormat}}\\
    \hline
\end{longtable}

\endlatexonly
	
*/
