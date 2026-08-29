const emailModule = require('./email')
const {Errors, code_prefix} = require("./const");
const {VerifyService} = require("./proto");
const grpc = require('@grpc/grpc-js')

const crypto = require('crypto');
const {server_port} = require("./config");
function generateVerifyCode() {
    return String(crypto.randomInt(100000, 1000000));
}
async function handleGetVerifyCode(call, callback) {
    console.log("email is ", call.request.email);
    try{
        const verifyCode = generateVerifyCode();
        const text_str = `您的验证码为 ${verifyCode}，请在 3 分钟内完成注册。`;
        //下一步保存验证码到redis

        //发送邮件
        let mailOptions = {
            from: 'zxzzsdb@163.com',
            to: call.request.email,
            subject: '验证码',
            text: text_str,
        };

        let send_res = await emailModule.SendMail(mailOptions);
        console.log("send res is ", send_res);

        callback(null, { email:  call.request.email,
            error:Errors.Success
        });


    }catch(error){
        console.log("catch error is ", error)

        callback(null, { email:  call.request.email,
            error:Errors.Exception
        });
    }

}

function main() {
    var server = new grpc.Server()
    server.addService(VerifyService.service, { GetVerifyCode: handleGetVerifyCode })
    server.bindAsync(server_port, grpc.ServerCredentials.createInsecure(), () => {
        console.log('grpc server started')
    })
}

main()