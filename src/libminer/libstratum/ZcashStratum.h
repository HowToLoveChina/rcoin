﻿#pragma once
// Copyright (c) 2016 Jack Grigg <jack@z.cash>
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "arith_uint256.h"
#include "primitives/block.h"
#include "uint256.h"
//#include "util.h"

#include <boost/signals2.hpp>
//#include <boost/signals.hpp>
//#include <boost/thread.hpp>
#include <thread>
#include <mutex>

#include "json/json_spirit_value.h"

#include "ISolver.h"
#include "speed.hpp"

using namespace json_spirit;

extern int use_avx;
extern int use_avx2;

struct EquihashSolution
{
    uint256 nonce;
	std::string time;
	size_t nonce1size;
    std::vector<unsigned char> solution;

    EquihashSolution(uint256 n, std::vector<unsigned char> s, std::string t, size_t n1s)
		: nonce{ n }, nonce1size{ n1s } { solution = s; time = t; }

    std::string toString() const { return nonce.GetHex(); }
};

struct ZcashJob
{
    std::string job;
    CBlockHeader header;
    std::string time;
    size_t nonce1Size;
    arith_uint256 nonce2Space;
    arith_uint256 nonce2Inc;
    arith_uint256 serverTarget;
    bool clean;

    bool equals(const ZcashJob& a) const { return job == a.job; }

    // Access Stratum flags
    std::string jobId() const { return job; }
    bool cleanJobs() const { return clean; }

    void setTarget(std::string target);
};

inline bool operator==(const ZcashJob& a, const ZcashJob& b)
{
    return a.equals(b);
}

typedef boost::signals2::signal<void (std::shared_ptr<ZcashJob>)> NewJob_t;


class ZcashMiner
{
    int nThreads;
	std::vector<std::thread> minerThreads;
    //boost::thread_group* minerThreads;
    uint256 nonce1;
    size_t nonce1Size;
    arith_uint256 nonce2Space;
    arith_uint256 nonce2Inc;
    std::function<bool(const EquihashSolution&, const std::string&)> solutionFoundCallback;
	bool m_isActive;

	std::vector<std::unique_ptr<ISolver>> solvers;

	Speed* speed;

public:
    NewJob_t NewJob;
	std::vector<bool> minerThreadActive;

	ZcashMiner(Speed*, std::vector<std::unique_ptr<ISolver>> &&i_solvers);
	~ZcashMiner();

    std::string userAgent();
    void start();
    void stop();
	bool isMining() { return m_isActive; }
	void setServerNonce(const std::string& n1str);
    std::shared_ptr<ZcashJob> parseJob(const Array& params);
    void setJob(std::shared_ptr<ZcashJob> job);
	void onSolutionFound(const std::function<bool(const EquihashSolution&, const std::string&)> callback);
	void submitSolution(const EquihashSolution& solution, const std::string& jobid);
    void acceptedSolution(bool stale);
    void rejectedSolution(bool stale);
    void failedSolution();
};

