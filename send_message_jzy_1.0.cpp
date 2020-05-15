/* RAFT节点类发送函数部分 */
/* jzy 2020/5/15 */

/* 函数声明 */

void send_request_vote();  // 发送投票请求，仅candidate
void send_append_entries(); // 发送附加日志，仅leader
bool copy_log(const LOG* log_1, const LOG* log_2, int start_1, int start_2, int length); // 复制日志

void answer_request_vote(int receiver_id, bool voteGranted); // 回复投票请求
void answer_append_entries(int receiver_id, bool success); // 回复附加日志

void answer_examine_node(); // 回复审查节点？？回复啥


/* 函数定义 */

/* RAFT节点间发送消息 */

/*!
 *  @brief  节点间发送投票请求
 *  @param  void
 *  @return void
 */    
void send_request_vote()
{
    int term = currentTerm;
    int candidateId = nodeId;
    int lastLogIndex = commitIndex;
    int lastLogTerm = log[commitIndex].term;
    
    for(int i=0; i<nodeTotal; ++i){
        if(i == nodeId)
            continue;

        // 再写入txt
    }
}


/*!
 *  @brief  节点间发送附加日志
 *  @param  void
 *  @return void
 */  
void send_append_entries()
{
    int term = currentTerm;
    int leaderId = nodeId;
    int prevLogIndex;
    int prevLogTerm;
    int lastMatch;      // 上次该节点同步到的日志索引
    int leaderCommit = commitIndex;
    LOG* entries = nullptr;     // 如果不追求同步效率是不是每次可以只传一个？

    for(int i=0; i<nodeTotal; ++i){
        if(i == nodeId)
            continue;

        // 每个节点参数不同
        prevLogIndex = nextIndex[i] - 1;
        prevLogTerm = log[prevLogIndex].term;
        lastMatch = matchIndex[i];
        entries = new LOG[leaderCommit-lastMatch];  // 长度究竟多长
        copy_log(entries, log, 0, lastMatch, leaderCommit-lastMatch);

        // 写入txt

        // 重置
        delete [] entries;
    }
}

/*!
 *  @brief  复制日志，内部函数
 *  @param  log_1       新数组   
 *          log_2       原数组
 *          start_1     新数组下标起点
 *          start_2     原数组下标起点
 *          length      复制长度       
 *  @return bool        是否复制成功
 */  
bool copy_log(const LOG* log_1, const LOG* log_2, int start_1, int start_2, int length);
{
    //  等日志结构先写好
    return false;
}


/*!
 *  @brief  回复节点间投票请求
 *  @param  voteGranted     是否同意投票
 *          receiver_id     接收节点id                   
 *  @return void
 */  
void answer_request_vote(int receiver_id, bool voteGranted)
{
    int term = currentTerm;

    // 写入txt
}


/*!
 *  @brief  回复节点间投票请求
 *  @param  success         是否匹配日志  
 *          receiver_id     接收节点id 
 *  @return void
 */  
void answer_append_entries(int receiver_id, bool success)
{
    int term = currentTerm;

    // 写入txt
}


/* 发送消息给审核节点 */

/* 是否有写成一个函数的必要，反正审核节点必须要等 */
/* 我们回复完了再进行下一步操作，可以之间把反馈放 */
/* 在审核发送给RAFT节点的函数的返回值中 */
void answer_examine_node(); 

