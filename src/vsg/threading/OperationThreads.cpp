/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/threading/OperationThreads.h>

using namespace vsg;

OperationThreads::OperationThreads(uint32_t numThreads, bool waitavailability, ref_ptr<Active> in_active) :
    active(in_active)
{
    if (!active) active = new Active;
    queue = new OperationQueue(active);

    auto runwhenavailable = [](ref_ptr<OperationQueue> q, ref_ptr<Active> a) {
        while (*(a))
        {
            ref_ptr<Operation> operation = q->take_when_avilable();
            if (operation)
            {
                operation->run();
            }
        }
    };
    auto run = [](ref_ptr<OperationQueue> q, ref_ptr<Active> a) {
        while (*(a))
        {
            ref_ptr<Operation> operation = q->take_when_avilable();
            if (operation)
            {
                operation->run();
            }
        }
    };

    if(waitavailability)
    {
        for (size_t i = 0; i < numThreads; ++i)
            threads.emplace_back(std::thread(runwhenavailable, std::ref(queue), std::ref(active)));
    }
    else
    {
        for (size_t i = 0; i < numThreads; ++i)
            threads.emplace_back(std::thread(run, std::ref(queue), std::ref(active)));
    }
}

void OperationThreads::run()
{
    while (ref_ptr<Operation> operation = queue->take())
    {
        operation->run();
    }
}

void OperationThreads::stop()
{
    active->active.exchange(false);
    for (auto& thread : threads)
    {
        thread.join();
    }
    threads.clear();
}
