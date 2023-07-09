import sys
sys.path.append("TexSoup\\TexSoup.zip")
print(sys.path)
import TexSoup
print(TexSoup)

soup = TexSoup.TexSoup("""
\begin{document}

\section{Hello \textit{world}.}

\subsection{Watermelon}

(n.) A sacred fruit. Also known as:

\begin{itemize}
\item red lemon
\item life
\end{itemize}

Here is the prevalence of each synonym.

\begin{tabular}{c c}
red lemon & uncommon \\
life & common
\end{tabular}

\end{document}
""")

print(soup.section.string)