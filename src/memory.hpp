#ifndef MEMORY_HPP
#define MEMORY_HPP

void initMemMgr();

#ifdef _DEBUG

void traceLeaks();
#define TRACE_LEAKS() traceLeaks()

#else

#define TRACE_LEAKS()

#endif

#endif
